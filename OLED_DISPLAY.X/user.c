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


volatile struct FLAGS flags;

//CORE VALUE VARIABLES
volatile struct VEHICLE vehicle;
volatile VCU_STATE_A VCU_A;
volatile STW_STATE_BUTTONS Steering_Wheel;


//DISPLAY VARIABLES
uint8_t tx_buf[256 * 64 / 2];

bool display_off = false;
bool stw_fn1_prev = false;

CAN_MSG_OBJ RECmsg;
uint8_t data_rec_message[8];

bool _CAN_Read_Helper()  {
    for (uint8_t i = 0; i < 8; i++) {
        data_rec_message[i] = 0;
    }
    RECmsg.data = data_rec_message;
    return CAN1_Receive(&RECmsg);
}

void Handle_Display_Off() {
    if (display_off) {
//        TMR1_Stop();
        C1FEN1 = 0x08;
        SSD1322_API_sleep_on();
    } else {
        C1FEN1 = 0x1F;
        SSD1322_API_sleep_off();
//        TMR1_Start();
    }
}

void CAN_Receive(void){
    while(_CAN_Read_Helper()){        
        if(RECmsg.msgId==0x129){            // VCU / Switch Table
            VCU_A.bits = RECmsg.data[0];
        }
        else if(RECmsg.msgId==0x123){       // Encoder
            vehicle.rpm = ((uint16_t)(RECmsg.data[0] << 8) | (uint16_t)RECmsg.data[1]) / 100.0F;
        }
        else if (RECmsg.msgId==0x150) {     // VCU Calculated State
            uint8_t new_lap_num = RECmsg.data[0];
            
            if (new_lap_num != vehicle.lap_number) {
                vehicle.prev_lap_joule = vehicle.lap_joule;
                vehicle.lap_joule = vehicle.total_joule;
            }
            
            vehicle.lap_number = new_lap_num;
            vehicle.lap_sec =  ((RECmsg.data[1] << 8) | RECmsg.data[2]) / 100.0F;
            vehicle.distance = ((RECmsg.data[3] << 8) | RECmsg.data[4]) / 20.0F;
            vehicle.delta_time_sec = ((RECmsg.data[6] << 8) | RECmsg.data[7]) / 100.0F;
        }
        else if (RECmsg.msgId==0x190) {     // Steering Wheel
            Steering_Wheel.bits = RECmsg.data[0];            
            if (Steering_Wheel.FN1 == true && stw_fn1_prev == false) {
                display_off = !display_off;
                Handle_Display_Off();
            }
            stw_fn1_prev = Steering_Wheel.FN1;
        }
        else if (RECmsg.msgId==0x350) {     // Joulemeter
            uint32_t new_joule = (RECmsg.data[0] << 24) | (RECmsg.data[1] << 16) | (RECmsg.data[2] << 8) | (RECmsg.data[3]);
            vehicle.total_joule = new_joule;
        }
    }
}

void CalculateDisplayValues(void){
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
        float abs_delta = vehicle.delta_time_sec;
        if(abs_delta < 0) abs_delta *= -1;
        if(abs_delta<10){ // fixed decimal point with padding
            draw_text(tx_buf, "00", 220, 16, 1);
            draw_text(tx_buf, itoa(abs_delta), 232, 16, brightness);
        } else if(abs_delta<100){
            draw_char(tx_buf, '0', 220, 16, 1);
            draw_text(tx_buf, itoa(abs_delta), 226, 16, brightness);
        }else{
            draw_text(tx_buf, itoa(abs_delta), 220, 16, brightness);
        }
        draw_char(tx_buf, '.', 238, 16, brightness);
        draw_text(tx_buf, itoa((abs_delta-(uint16_t)abs_delta)*10), 244, 16, brightness);
        draw_text(tx_buf, "S", 250, 16, brightness);
        
        
    // LAST LAP JOULE
        select_font(&Font5x7FixedMono);
        float lap_joule_display = vehicle.lap_joule - vehicle.prev_lap_joule;
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

void User_Idle_Normal(void){
    // make sure the CAN doesn't go to sleep
//    C1CTRL1bits.CSIDL = 0;
    
    // Reset WatchDogTimer and Idle wakeup bits
    RCONbits.IDLE = 0;
    RCONbits.WDTO = 0;
    // Disable DMA interrupts in idle
//    IPC1bits.DMA0IP = 0;
//    IPC6bits.DMA2IP = 0;
    // ADC disabled in idle
//    AD1CON1bits.ADSIDL = 1;
    
    // Idle mode disables the CPU but keep peripherals running
    Idle();
    
    // Check what woke up the CPU
    if(RCONbits.IDLE == 1){
        RCONbits.IDLE = 0;
        if(RCONbits.WDTO == 1){
            RCONbits.WDTO = 0;
        }
        ClrWdt();
//        IPC1bits.DMA0IP = 1;
//        IPC6bits.DMA2IP = 1;
    }
}
