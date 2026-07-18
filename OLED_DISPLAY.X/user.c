/*
 * File:   user.c
 * Author: administrator
 *
 * Created on October 21, 2020, 10:49 AM
 */

#define SLAVE_I2C_GENERIC_RETRY_MAX 100
#define SLAVE_I2C_GENERIC_DEVICE_TIMEOUT 50

#include <math.h>

#include "xc.h"
#include "user.h"
#include "speed_graph_lut.h"
#include "mcc_generated_files/tmr1.h"


volatile struct FLAGS flags = {};

//CORE VALUE VARIABLES
volatile struct VEHICLE vehicle = {};
volatile CAN_Bytes encoder = {};
//volatile CAN_Bytes battery_current = {};
//volatile CAN_Bytes battery_voltage = {};
volatile VCU_STATE_A VCU_A = {};
//volatile VCU_STATE_B VcuState_B = {};
volatile STW_STATE_BUTTONS Steering_Wheel = {};

double rpm_m_average = 0;
double rpm_avg[3];
double rpm_avg_sum = 0;

volatile uint8_t SPI_data[8]={0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8};

//DISPLAY VARIABLES
uint8_t tx_buf[256 * 64 / 2];
//uint8_t tx_buf[64 * 64 / 2];
//uint16_t offset = 0;
int display_update_cnt = 0;

uint8_t c_top_brightness = 7;
uint8_t c_mid_brightness = 11;
uint8_t c_bottom_brightness = 15;

uint16_t can_msg_num = 0;

//ADC
ADC1_CHANNEL left_phototrans = channel_AN15;
ADC1_CHANNEL right_phototrans = channel_AN26;
uint16_t left_brightness = 0;
uint16_t right_brightness = 0;
uint16_t adaptive_brightness = 0;
uint16_t prev_adaptive_brightness = 0;

bool display_off = false;
bool stw_fn1_prev = false;

//TIMER VARIABLES
uint32_t tmr1_cnt = 0;
bool tmr1_flag = false;

uint32_t tmr1_1s_cnt = 0;
bool tmr1_1s_flag = false;

uint32_t display_hz_cnt = 0;
uint32_t display_hz = 0;

uint16_t update_cnt_100ms = 0;

uint16_t debounce_500ms = 0;

uint32_t cnt = 0;
bool cnt_flag = false;

//CAN VARIABLES
//CAN_MSG_OBJ RECmsg;
//uint8_t data_rec_message[8];

//CAN_MSG_OBJ TRANSmsg;
//uint8_t data_trans_message[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};

float vesc_voltage = 0;
float vesc_current = 0;

CAN_MSG_OBJ RECmsg;
uint8_t data_rec_message[8];

bool _CAN_Read_Helper()  {
    for(size_t i = 0; i < 8; i++) data_rec_message[i] = 0;
    RECmsg.data = data_rec_message;
    return CAN1_Receive(&RECmsg);
}

void _Handle_Display_Off() {
    if (display_off) {
        C1FEN1 = 0x08;
        fill_buffer(tx_buf, 0);
        send_buffer_to_OLED(tx_buf, 0, 0);
        TMR1_Stop();
    } else {
        C1FEN1 = 0x7F;
        TMR1_Start();
    }
}

void CAN_Receive(void){
    while(_CAN_Read_Helper()){        
        if(RECmsg.msgId==0x129){ //saves sw table
            VCU_A.bits = RECmsg.data[0];
        }
        else if(RECmsg.msgId==0x123){ //saves RPM value
            update_cnt_100ms = 0;
            flags.update_synced = true;
            encoder.HighByte = RECmsg.data[0];
            encoder.LowByte = RECmsg.data[1];
        }
        else if (RECmsg.msgId==0x150) {
            vehicle.lap_number = RECmsg.data[0];
            vehicle.lap_sec =  ((RECmsg.data[1] << 8) | RECmsg.data[2]) / 100.0F;
            vehicle.distance = ((RECmsg.data[3] << 8) | RECmsg.data[4]) / 20.0F;
            vehicle.delta_time_sec = ((RECmsg.data[6] << 8) | RECmsg.data[7]) / 100.0F;
        }
        else if (RECmsg.msgId==0x190) {
            Steering_Wheel.bits = RECmsg.data[0];            
            if (Steering_Wheel.FN1 == true && stw_fn1_prev == false) {
                display_off = !display_off;
                _Handle_Display_Off();
            }
            stw_fn1_prev = Steering_Wheel.FN1;
        }
        else if (RECmsg.msgId==0x1B51) {
            vesc_voltage = ((RECmsg.data[5]<<8) | RECmsg.data[4]) / 10.0F;
        }
        else if (RECmsg.msgId==0x1051) {
            vesc_current = ((RECmsg.data[5]<<8) | RECmsg.data[4]) / 10.0F;
        }
        else if (RECmsg.msgId==0x4028001) {
            vehicle.voltage = ((RECmsg.data[0]<<8) | RECmsg.data[1]) / 10.0F;
            vehicle.current = (((int16_t)(RECmsg.data[2]<<8) | RECmsg.data[3])) / 10.0F - 30000;
        }
//        else if(RECmsg.msgId==0x700){ //saves battery current and voltage values
//            battery_current.HighByte = RECmsg.data[0];
//            battery_current.LowByte = RECmsg.data[1];
//            battery_voltage.HighByte = RECmsg.data[2];
//            battery_voltage.LowByte = RECmsg.data[3];
//        }
    }
}

void CalculateDisplayValues(void){
    //RPM moving average
    rpm_avg[0] = ((double)encoder.Word)/100;
    rpm_avg[1] = rpm_avg[0];
    rpm_avg[2] = rpm_avg[1];
    rpm_avg_sum = rpm_avg[0] + rpm_avg[1] + rpm_avg[2];
    if(rpm_avg_sum == 0){
        vehicle.rpm = 0;
    }else{
        vehicle.rpm = rpm_avg_sum/3;
    }
    
    //Speed
    vehicle.speed = vehicle.rpm * SPEED_MULT_FACTOR;
} 

// brightness 0-15
void UpdateDisplay(uint8_t brightness){
    
    //clear display buffer
        fill_buffer(tx_buf, 0);
    
    // SPEED
        select_font(&FreeSans9pt7b);
        if(vehicle.speed<10){ // fixed decimal point with padding
            draw_char(tx_buf, '0', 0, 14, 1);
            draw_text(tx_buf, itoa(vehicle.speed), 10, 14, brightness);
        }else{
            draw_text(tx_buf, itoa(vehicle.speed), 0, 14, brightness);
        }
        draw_char(tx_buf, '.', 20, 14, brightness);
        draw_text(tx_buf, itoa((vehicle.speed-(uint8_t)vehicle.speed)*10), 24, 14, brightness);
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "KPH", 36, 9, brightness);
        
    // LAPS
        select_font(&FreeSans9pt7b);
        if (vehicle.lap_number < 10) { // padding below two digits
            draw_char(tx_buf, '0', 65, 14, 1);
            draw_text(tx_buf, itoa(vehicle.lap_number), 75, 14, brightness);
        } else {
            draw_text(tx_buf, itoa(vehicle.lap_number), 65, 14, brightness);
        }
        draw_char(tx_buf, '/', 86, 14, brightness);
        if (TOTAL_LAPS < 10) {
            draw_char(tx_buf, '0', 92, 14, 1);
            draw_text(tx_buf, itoa(TOTAL_LAPS), 102, 14, brightness);
        } else {
            draw_text(tx_buf, itoa(TOTAL_LAPS), 92, 14, brightness);
        }
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "LAP", 113, 9, brightness);
           
        
    // TIME
        select_font(&FreeSans9pt7b);
        if(vehicle.lap_sec<10){ // fixed decimal point with padding
            draw_text(tx_buf, "00", 140, 14, 1);
            draw_text(tx_buf, itoa(vehicle.lap_sec), 160, 14, brightness);
        } else if(vehicle.lap_sec<100){
            draw_char(tx_buf, '0', 140, 14, 1);
            draw_text(tx_buf, itoa(vehicle.lap_sec), 150, 14, brightness);
        }else{
            draw_text(tx_buf, itoa(vehicle.lap_sec), 140, 14, brightness);
        }
        draw_char(tx_buf, '.', 170, 14, brightness);
        draw_text(tx_buf, itoa((vehicle.lap_sec-(uint16_t)vehicle.lap_sec)*10), 174, 14, brightness);
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "SEC", 186, 9, brightness);
              
    // DELTA TIME
        select_font(&Font5x7FixedMono);
        if (vehicle.delta_time_sec > 0) {
            draw_char(tx_buf, '+', 214, 16, brightness);
        } else if (vehicle.delta_time_sec < 0) {
            draw_char(tx_buf, '-', 214, 16, brightness);
        }
        if(vehicle.delta_time_sec<10){ // fixed decimal point with padding
            draw_text(tx_buf, "00", 220, 16, 1);
            draw_text(tx_buf, itoa(vehicle.delta_time_sec), 232, 16, brightness);
        } else if(vehicle.delta_time_sec<100){
            draw_char(tx_buf, '0', 220, 16, 1);
            draw_text(tx_buf, itoa(vehicle.delta_time_sec), 226, 16, brightness);
        }else{
            draw_text(tx_buf, itoa(vehicle.delta_time_sec), 220, 16, brightness);
        }
        draw_char(tx_buf, '.', 238, 16, brightness);
        draw_text(tx_buf, itoa((vehicle.delta_time_sec-(uint16_t)vehicle.delta_time_sec)*10), 244, 16, brightness);
        draw_text(tx_buf, "S", 250, 16, brightness);
        
        
    // LAST LAP JOULE
        select_font(&Font5x7FixedMono);
        float lap_joule_display = vehicle.prev_lap_joule;
        uint8_t lap_joule_offset;
        if (lap_joule_display == 0) {
            lap_joule_offset = 1;
        } else {
            lap_joule_offset = ((uint8_t)log10f(lap_joule_display))+1;
        }
        for (uint8_t i = 5; i > lap_joule_offset; i--) {
            draw_char(tx_buf, '0', 250-i*6, 7, 2);
        }
        draw_text(tx_buf, itoa(lap_joule_display), 250-lap_joule_offset*6, 7, brightness);
        draw_char(tx_buf, 'J', 250, 7, brightness);
        
    // Distance
//        select_font(&Font5x7FixedMono);
//        draw_text(tx_buf, "DIST:", 78, 46, brightness);
//        draw_text(tx_buf, itoa(vehicle.distance), 109, 46, brightness);
        
        draw_hline(tx_buf, 18, 0, DISPLAY_WIDTH-1, 2);
        
        
    // SPEED GRAPH
        
    // scale ladder
        draw_vline(tx_buf, 15, 22, 62, brightness);
        draw_pixel(tx_buf, 14, 22, brightness);
        draw_pixel(tx_buf, 13, 22, brightness);
        draw_pixel(tx_buf, 14, 32, brightness);
        draw_pixel(tx_buf, 14, 42, brightness);
        draw_pixel(tx_buf, 14, 52, brightness);
        draw_pixel(tx_buf, 14, 62, brightness);
        draw_pixel(tx_buf, 13, 62, brightness);
        
    // speed min/max
        draw_text(tx_buf, itoa(LUT_MAX_SPEED), 0, 28, brightness);
        draw_text(tx_buf, itoa(LUT_MIN_SPEED), 0, 63, brightness);
        
    // speed arrow
        uint8_t arrow_offset = map_value(vehicle.speed, LUT_MIN_SPEED, LUT_MAX_SPEED, 0, 40);
        draw_vline(tx_buf, 17, 59-arrow_offset, 65-arrow_offset, brightness);
        draw_vline(tx_buf, 18, 60-arrow_offset, 64-arrow_offset, brightness);
        draw_vline(tx_buf, 19, 61-arrow_offset, 63-arrow_offset, brightness);
        draw_pixel(tx_buf, 20, 62-arrow_offset, brightness);
        
        for(uint16_t i = 21; i < DISPLAY_WIDTH; i++){
            uint16_t lut_index = (vehicle.distance/LUT_DISTANCE_STEP)+i-21;
            if (lut_index > 0 && lut_index < LUT_SIZE) {
                float lut_val = 62-map_value(lut_dist_kmh[lut_index], LUT_MIN_SPEED, LUT_MAX_SPEED, 0, 40);
                if (lut_acc[lut_index] > 0) {
                    draw_vline(tx_buf, i, lut_val, 64, 1);
                }
                draw_pixel(tx_buf, i, lut_val, brightness);
            }
        }
        
        send_buffer_to_OLED(tx_buf, 0, 0);
        
        display_hz_cnt++;
}


// returns pointer to ASCII string in a static buffer
char* itoa(uint32_t value) 
 {
     static char buffer[12];        // 12 bytes is big enough for an INT32
     uint32_t original = value;        // save original value
 
     int c = sizeof(buffer)-1;
 
     buffer[c] = 0;                // write trailing null in last byte of buffer    
 
     if (value < 0)                 // if it's negative, note that and take the absolute value
         value = -value;
     
     do                             // write least significant digit of value that's left
     {
         buffer[--c] = (value % 10) + '0';    
         value /= 10;
     } while (value);
 
     if (original < 0) 
         buffer[--c] = '-';
 
     return &buffer[c];
 }

int32_t map_value(int32_t x, int32_t min_x, int32_t max_x, int32_t min_to, int32_t max_to) {
    int32_t new_x = x;
    if (new_x < min_x) new_x = min_x;
    else if (new_x > max_x) new_x = max_x;
    return (new_x - min_x) * (max_to - min_to) / (max_x - min_x) + min_to;
}

void GetBrightnessADC(void){
    ADC1_Enable();
    ADC1_ChannelSelect(left_phototrans);
    ADC1_SoftwareTriggerEnable();
    for(int i=0;i <1000;i++);//Delay
    ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(left_phototrans));
    left_brightness = ADC1_ConversionResultGet(left_phototrans);
    
    ADC1_ChannelSelect(right_phototrans);
    ADC1_SoftwareTriggerEnable();
    for(int i=0;i <100;i++);//Delay
    ADC1_SoftwareTriggerDisable();
    while(!ADC1_IsConversionComplete(right_phototrans));
    right_brightness = ADC1_ConversionResultGet(right_phototrans);
    ADC1_Disable(); 
    
    adaptive_brightness = (left_brightness+right_brightness)/4;
    if(adaptive_brightness>15){
        adaptive_brightness = 15;
    }else if(adaptive_brightness<3){
        adaptive_brightness = 3;
    }
    
    if(adaptive_brightness<(prev_adaptive_brightness+3) && adaptive_brightness>(prev_adaptive_brightness-3)){ //to avoid flickering
        adaptive_brightness = prev_adaptive_brightness;
    }
}

void SpeedArrow(double double_speed, uint8_t brightness){
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-6, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-5, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-5, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-4, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-4, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-4, brightness);
    draw_pixel(tx_buf, 162, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-3, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-3, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-3, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-3, brightness);
    draw_pixel(tx_buf, 163, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-2, brightness);
    draw_pixel(tx_buf, 162, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-2, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-2, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-2, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-2, brightness);
    draw_pixel(tx_buf, 164, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-1, brightness);
    draw_pixel(tx_buf, 163, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-1, brightness);
    draw_pixel(tx_buf, 162, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-1, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-1, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-1, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET-1, brightness);
    draw_pixel(tx_buf, 166, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET, brightness);
    draw_pixel(tx_buf, 165, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET, brightness);
    draw_pixel(tx_buf, 164, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET, brightness);
    draw_pixel(tx_buf, 163, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET, brightness);
    draw_pixel(tx_buf, 162, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET, brightness);
    draw_pixel(tx_buf, 164, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+1, brightness);
    draw_pixel(tx_buf, 163, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+1, brightness);
    draw_pixel(tx_buf, 162, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+1, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+1, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+1, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+1, brightness);
    draw_pixel(tx_buf, 163, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+2, brightness);
    draw_pixel(tx_buf, 162, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+2, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+2, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+2, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+2, brightness);
    draw_pixel(tx_buf, 162, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+3, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+3, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+3, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+3, brightness);
    draw_pixel(tx_buf, 161, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+4, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+4, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+4, brightness);
    draw_pixel(tx_buf, 160, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+5, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+5, brightness);
    draw_pixel(tx_buf, 159, ((int)double_speed)*(-1)+SPD_GRAPH_OFFSET+6, brightness);
    
}

void AccChevron(uint16_t x_pos, uint16_t y_pos){
    if(c_top_brightness<2) c_top_brightness = 15;
    if(c_mid_brightness<2) c_mid_brightness = 15;
    if(c_bottom_brightness<2) c_bottom_brightness = 15;
    
    draw_line(tx_buf, x_pos, y_pos, x_pos+8, y_pos-8, c_top_brightness);
    draw_line(tx_buf, x_pos, y_pos-1, x_pos+8, y_pos-9, c_top_brightness);
    draw_line(tx_buf, x_pos+16, y_pos, x_pos+8, y_pos-8, c_top_brightness);
    draw_line(tx_buf, x_pos+16, y_pos-1, x_pos+8, y_pos-9, c_top_brightness);
    
    draw_line(tx_buf, x_pos, y_pos-4, x_pos+8, y_pos-12, c_mid_brightness);
    draw_line(tx_buf, x_pos, y_pos-5, x_pos+8, y_pos-13, c_mid_brightness);
    draw_line(tx_buf, x_pos+16, y_pos-4, x_pos+8, y_pos-12, c_mid_brightness);
    draw_line(tx_buf, x_pos+16, y_pos-5, x_pos+8, y_pos-13, c_mid_brightness);
    
    draw_line(tx_buf, x_pos, y_pos-8, x_pos+8, y_pos-16, c_bottom_brightness);
    draw_line(tx_buf, x_pos, y_pos-9, x_pos+8, y_pos-17, c_bottom_brightness);
    draw_line(tx_buf, x_pos+16, y_pos-8, x_pos+8, y_pos-16, c_bottom_brightness);
    draw_line(tx_buf, x_pos+16, y_pos-9, x_pos+8, y_pos-17, c_bottom_brightness);
    
    c_top_brightness -= 2;
    c_mid_brightness -= 2;
    c_bottom_brightness -= 2;
}

void User_Idle_Normal(void){
    // Reset WatchDogTimer and Idle wakeup bits
    RCONbits.IDLE = 0;
    RCONbits.WDTO = 0;
    // Disable DMA interrupts in idle
    IPC1bits.DMA0IP = 0;
    IPC6bits.DMA2IP = 0;
    // ADC disabled in idle
    AD1CON1bits.ADSIDL = 1;
    
    // Idle mode disables the CPU but keep peripherals running
    Idle();
    
    // Check what woke up the CPU
    if(RCONbits.IDLE == 1){
        RCONbits.IDLE = 0;
        if(RCONbits.WDTO == 1){
            RCONbits.WDTO = 0;
        }
        ClrWdt();
        IPC1bits.DMA0IP = 1;
        IPC6bits.DMA2IP = 1;
    }
}






