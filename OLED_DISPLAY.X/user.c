/*
 * File:   user.c
 * Author: administrator
 *
 * Created on October 21, 2020, 10:49 AM
 */

#define SLAVE_I2C_GENERIC_RETRY_MAX 100
#define SLAVE_I2C_GENERIC_DEVICE_TIMEOUT 50

#include "xc.h"
#include "user.h"


volatile struct FLAGS flags = {};

//CORE VALUE VARIABLES
volatile struct VEHICLE vehicle = {};
volatile CAN_Bytes encoder = {};
//volatile CAN_Bytes battery_current = {};
//volatile CAN_Bytes battery_voltage = {};
volatile VCU_STATE_A VcuState_A = {};
//volatile VCU_STATE_B VcuState_B = {};
volatile STW_STATE_BUTTONS StwState = {};

double rpm_m_average = 0;
double rpm_avg[3];
double rpm_avg_sum = 0;

volatile uint8_t SPI_data[8]={0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8};

//DISPLAY VARIABLES
uint8_t tx_buf[256 * 64 / 2];
//uint8_t tx_buf[64 * 64 / 2];
double offset = 0;
int display_update_cnt = 0;

uint8_t c_top_brightness = 7;
uint8_t c_mid_brightness = 11;
uint8_t c_bottom_brightness = 15;

uint16_t can_msg_num = 0;

//lut size must be changed in user.h too
//uint8_t lut[1157]={0,13,18,22,26,29,31,34,36,38,40,42,44,46,47,49,50,52,53,55,56,57,58,59,60,61,62,63,63,63,63,63,62,62,62,62,62,62,62,63,63,64,64,65,65,66,66,66,66,66,66,66,66,66,66,66,65,65,65,65,65,65,65,64,64,64,64,64,64,64,63,63,63,63,63,63,62,62,63,63,64,64,65,65,65,65,65,64,64,64,64,64,64,64,64,63,63,63,63,63,63,63,63,63,63,64,65,65,65,65,65,65,65,65,65,65,64,64,64,64,64,64,64,64,64,64,64,64,64,64,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,62,62,62,62,62,62,62,62,62,61,61,61,61,61,61,60,60,60,60,60,59,59,59,59,59,58,58,58,58,57,57,57,57,57,57,56,56,56,56,56,56,56,55,55,55,55,55,55,54,54,54,54,54,54,54,53,53,53,53,53,53,53,53,52,52,52,52,52,52,52,52,51,51,51,51,51,51,51,51,50,50,50,50,50,50,50,49,49,49,49,49,49,49,48,48,48,48,48,48,47,47,47,47,47,46,46,46,46,46,46,45,45,45,45,45,45,45,44,44,44,45,46,48,50,51,52,52,51,51,51,51,51,51,51,50,50,50,50,50,50,49,49,49,49,49,49,49,48,48,48,48,48,48,47,47,47,47,47,47,46,46,46,46,46,46,46,45,45,45,45,45,45,45,44,44,44,44,44,44,43,43,43,43,43,43,42,42,42,42,42,42,41,41,41,41,41,41,40,40,40,40,40,40,39,39,39,39,39,39,38,39,41,43,45,46,47,47,47,47,46,46,46,46,46,46,46,45,45,45,45,45,45,44,44,44,44,44,44,43,43,43,43,43,43,42,42,42,42,42,42,41,41,41,41,41,41,40,40,40,40,40,39,39,39,39,39,39,38,38,38,38,37,37,37,37,37,39,41,43,45,45,45,45,45,45,45,44,44,44,44,44,44,43,43,43,43,43,42,42,42,42,42,42,42,41,41,41,41,41,41,41,41,41,40,40,41,43,45,46,48,49,49,49,48,48,49,50,52,53,54,55,55,54,54,54,54,56,57,58,59,59,59,59,59,59,59,60,61,61,62,62,62,62,62,61,61,61,61,61,60,60,60,60,59,59,59,59,59,58,58,58,58,57,57,57,57,57,56,56,56,56,56,55,55,55,55,54,54,54,54,54,54,53,53,53,53,53,52,52,52,52,52,52,52,51,51,51,51,51,51,51,50,50,50,50,50,50,50,50,49,50,52,53,54,56,56,56,56,56,56,56,56,56,55,55,55,55,55,55,55,55,54,54,54,54,54,54,54,53,53,54,55,56,58,59,59,59,59,59,59,59,58,58,58,58,59,60,60,61,62,63,63,64,64,65,65,65,65,65,64,64,64,64,64,64,64,63,63,63,63,63,63,63,62,62,62,62,62,62,62,61,61,61,61,61,61,61,60,60,60,61,61,62,63,63,64,64,65,65,66,66,66,65,65,65,65,65,65,65,64,64,64,64,64,64,64,63,63,63,63,63,63,63,62,62,62,62,62,62,61,61,61,61,61,61,60,60,60,60,60,60,60,59,59,59,59,59,59,58,58,58,58,58,58,58,57,57,57,57,57,57,56,56,56,56,56,56,55,55,55,56,57,58,59,60,61,60,60,60,60,60,60,59,59,59,59,59,59,59,58,58,58,58,58,58,57,57,57,57,57,57,57,56,56,56,56,56,56,55,55,55,55,55,55,54,54,54,54,54,54,53,53,53,53,53,53,53,52,52,52,52,52,52,51,51,51,51,51,51,51,51,50,50,50,50,50,50,50,50,49,50,52,53,54,56,56,56,56,55,55,55,55,55,54,54,55,56,57,58,59,60,60,59,59,59,59,60,61,61,62,62,62,62,61,61,61,61,60,60,60,60,59,59,59,58,58,58,58,57,57,57,57,56,56,56,56,58,59,60,60,61,61,60,60,60,60,60,59,59,59,59,59,59,58,58,58,58,58,58,57,57,57,57,57,57,57,58,59,60,61,61,61,61,60,60,60,60,59,59,59,59,58,58,58,58,57,57,57,56,56,56,55,55,55,55,54,54,54,54,53,53,53,53,52,52,52,52,52,51,51,51,51,50,50,50,50,50,49,49,49,49,48,48,48,48,48,47,47,47,47,46,46,46,46,46,45,45,45,45,45,45,44,44,44,44,44,44,43,43,43,43,43,42,42,42,42,41,41,41,41,41,40,40,40,40,40,40,39,39,39,39,39,38,38,38,38,38,37,37,37,37,37,36,36,36,36,35,35,35,35,35,34,34,34,34,34,33,33,33,33,33,32,32,32,32,32,31,31,31,31,31,31,31,30,30,30,30,30,30,30,29,29,29,29,29,29,29,28,28,28,28,28,28,28,28,27,27,27,27,27,27,27,26,26,26,26,26,26,26,25,25,25,25,25,25,25,24,24,24,24,24,24,23,23,23,23,23,22,22,22,22,22,22,21,21,21,19,14,0};
uint8_t lut[642] = {0,13,18,22,26,29,31,33,34,35,35,36,37,37,38,39,39,40,41,41,42,43,43,44,44,45,45,46,47,47,48,48,49,49,49,49,49,48,48,49,49,50,50,51,51,52,52,53,53,53,53,53,52,52,52,52,51,51,51,51,52,52,53,53,54,54,54,55,55,54,54,54,53,53,53,52,52,51,51,51,50,50,50,49,49,49,48,48,48,48,49,49,49,49,49,48,48,48,47,47,47,47,47,47,48,48,49,50,50,51,51,52,53,53,54,54,55,55,55,55,55,55,55,55,55,55,55,55,55,55,56,56,57,57,58,58,59,59,60,60,60,59,59,59,59,59,59,59,59,59,59,59,59,58,58,58,58,58,58,58,58,58,58,58,58,57,57,57,57,57,57,57,57,57,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,54,54,54,54,54,54,54,54,54,54,53,53,53,53,53,53,53,53,53,54,54,55,55,56,56,56,56,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,52,52,52,52,52,52,52,52,52,52,52,52,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,54,54,54,54,54,54,54,54,54,54,54,54,54,54,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,55,55,55,55,55,55,54,54,54,54,54,53,53,53,52,52,52,52,51,51,51,51,51,51,52,52,52,53,53,53,54,54,54,53,53,53,53,52,52,52,51,51,52,52,52,53,53,54,54,54,55,55,55,54,54,54,53,54,54,54,55,55,55,54,54,54,54,54,54,55,55,55,56,56,56,56,56,56,56,55,55,55,55,54,54,54,53,53,53,53,52,52,52,52,52,51,51,51,51,51,50,50,50,50,50,50,50,50,51,52,52,52,52,52,52,52,52,52,52,51,51,51,51,51,51,51,51,51,51,51,51,51,51,50,50,50,50,50,50,50,50,49,49,49,49,48,48,48,47,47,47,46,46,45,45,45,44,44,43,43,43,42,42,41,41,41,40,40,40,39,39,39,38,38,38,38,37,37,37,36,36,36,36,35,35,35,35,34,34,34,34,33,33,33,32,32,32,32,31,31,31,30,30,30,30,29,29,29,29,28,28,28,27,27,27,26,26,25,18,0};
uint16_t lut_size = (sizeof(lut)/sizeof(lut[0]));

//ADC
ADC1_CHANNEL left_phototrans = channel_AN15;
ADC1_CHANNEL right_phototrans = channel_AN26;
uint16_t left_brightness = 0;
uint16_t right_brightness = 0;
uint16_t adaptive_brightness = 0;
uint16_t prev_adaptive_brightness = 0;

bool blank_toggle = false;
bool stw_fn1_debnc = false;

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
CAN_MSG_OBJ RECmsg;
uint8_t data_rec_message[8] = {0};

CAN_MSG_OBJ TRANSmsg;
uint8_t data_trans_message[8] = {0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48};

float vesc_voltage = 0;
float vesc_current = 0;


bool _CAN_Read_Helper()  {
    RECmsg.data = data_rec_message;
    return CAN1_Receive(&RECmsg);
}

void CAN_Receive(void){
    while(_CAN_Read_Helper()){
        
        if(RECmsg.msgId==0x129){ //saves sw table
            VcuState_A.bits = RECmsg.data[0];
        }
        else if(RECmsg.msgId==0x123){ //saves RPM value
            update_cnt_100ms = 0;
            flags.update_synced = true;
            encoder.HighByte = RECmsg.data[0];
            encoder.LowByte = RECmsg.data[1];
            if(!RECmsg.data[2] && RECmsg.data[0]){
                vehicle.reverse = true;
            }
        }
        else if (RECmsg.msgId==0x150) {
            vehicle.lap_number = RECmsg.data[0];
            vehicle.lap_sec =  ((RECmsg.data[1] << 8) | RECmsg.data[2]) / 100.0F;
            vehicle.distance = ((RECmsg.data[3] << 8) | RECmsg.data[4]) / 20.0F;
            vehicle.delta_time_sec = ((RECmsg.data[5] << 8) | RECmsg.data[6]) / 100.0F;
        }
        else if (RECmsg.msgId==0x190) {
            StwState.bits = RECmsg.data[0];
        }
        else if (RECmsg.msgId=0x1B51) {
            vesc_voltage = ((RECmsg.data[5]<<8) | RECmsg.data[4]) / 10.0F;
        }
        else if (RECmsg.msgId=0x1051) {
            vesc_current = ((RECmsg.data[5]<<8) | RECmsg.data[4]) / 10.0F;
        }
//        else if(RECmsg.msgId==0x700){ //saves battery current and voltage values
//            battery_current.HighByte = RECmsg.data[0];
//            battery_current.LowByte = RECmsg.data[1];
//            battery_voltage.HighByte = RECmsg.data[2];
//            battery_voltage.LowByte = RECmsg.data[3];
//        }
    }
}

void CAN_Transmit(void){
//    TRANSmsg.msgId = 0x111;
//    TRANSmsg.field.frameType = CAN_FRAME_DATA;
//    TRANSmsg.field.idType = CAN_FRAME_STD;
//    TRANSmsg.field.dlc = CAN_DLC_8;
//    TRANSmsg.data = data_trans_message;
//
//    CAN1_Transmit(CAN_PRIORITY_HIGH, &TRANSmsg);
}

void CalculateDisplayValues(void){
    
//    if (StwState.FUNCTION2 && StwState.FUNCTION2 != stw_fn1_debnc) {
//        blank_toggle = !blank_toggle;
//        stw_fn1_debnc = StwState.FUNCTION2;
//    }
//    if (blank_toggle) {
//        return;
//    }
    
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
    
    //exponential average
    //ema_s = (EMA_A * vehicle.rpm) + ((1-EMA_A) * ema_s);
    
    
    //Speed
    vehicle.speed = vehicle.rpm * SPEED_MULT_FACTOR;
//    if(BUTTON_L_GetValue()){//TESTING CODE START this is for testing, replace with the line above
//        vehicle.speed += 0.2;
//    }else{
//        if(vehicle.speed>0){
//            vehicle.speed -= 0.02;
//        }   
//    }//TESTING CODE END
    
    
    //Distance
//    vehicle.distance += ((vehicle.speed)/72); //values in meters | /72 if used it every 50 ms /3600 if used it every 1 ms
    
    //Voltage
//    vehicle.voltage = (double)battery_voltage.Word/1000;
    
    //New joule
    vehicle.joule = vesc_current*vesc_voltage*0.05;
    vehicle.lap_joules[vehicle.lap_number] += vehicle.joule;
    vehicle.lap_joule = vehicle.lap_joules[vehicle.lap_number];
    
    //Joule
//    vehicle.joule = (((double)((int16_t)battery_current.Word))/300+0.047) * (((double)battery_voltage.Word)/1000) * 0.05;
//    if(vehicle.lap_number > 0){
//        vehicle.lap_joule += vehicle.joule; //added every 50 ms
//        vehicle.total_joule += vehicle.joule;
//    }else{
//        vehicle.lap_joule = 0;
//    }
    
    
    
    //Lap number
//    if (VcuState_A.LAP==1 && !flags.debounce){
////    if(BUTTON_R_GetValue() && !flags.debounce){ //this is for testing, replace with the line above
//        vehicle.lap_number++;
//        flags.debounce = true;
//        vehicle.previous_lap_sec = vehicle.lap_sec;
//        vehicle.lap_sec = 0;
//        vehicle.lap_ms = 0;
//        vehicle.distance = 0.;
//        
//        if (vehicle.lap_number == TOTAL_LAPS + 2){
//            vehicle.lap_number = 0;
//        }
//        
//        //Delta Time
//        if(vehicle.lap_number < 2){
//            vehicle.delta_time_sec = 0;
//            vehicle.best_lap_joule = 0;
//        }else if(vehicle.lap_number == 2){
//            vehicle.delta_time_sec = vehicle.previous_lap_sec - OPTIMAL_LAP_TIME;
//            vehicle.best_lap_joule = vehicle.lap_joule;
//        }else if(vehicle.lap_number > 2){
//            vehicle.delta_time_sec = vehicle.previous_lap_sec - OPTIMAL_LAP_TIME + vehicle.delta_time_sec;
//            if(vehicle.lap_joule < vehicle.best_lap_joule){
//                vehicle.best_lap_joule = vehicle.lap_joule;
//            }
//        }
//        vehicle.lap_joule = 0;
//    }
//    
//    if(vehicle.lap_number==0){
//        vehicle.lap_sec = 0;
//        vehicle.distance = 0;
//    }

} 

// brightness 0-15
void UpdateDisplay(uint8_t brightness){
    
    //clear display buffer
        fill_buffer(tx_buf, 0);
        
//        if (blank_toggle) {
//            return;
//        }
           
    // SPEED
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "SPD", 0, 7, brightness);
        select_font(&FreeSans9pt7b);
        if(vehicle.speed<10){ //this part makes the decimal point fixed
            draw_text(tx_buf, itoa(vehicle.speed), 28, 12, brightness);
        }else{
            draw_text(tx_buf, itoa(vehicle.speed), 18, 12, brightness);
        }
        draw_char(tx_buf, '.', 38, 12, brightness);
        
        draw_text(tx_buf, itoa((vehicle.speed-(uint8_t)vehicle.speed)*10), 41, 12, brightness);
        
    // LAPS      
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "LAPS", 78, 7, brightness);
        select_font(&FreeSans9pt7b);
        if(vehicle.lap_number < 10){ //the lap number is in different place depending on the decimal value
            if (vehicle.lap_number == TOTAL_LAPS + 1){
                draw_text(tx_buf, "#", 112, 12, brightness); //there is a +1 lap to see the final energy consumption of the previous lap
            }else{
                draw_text(tx_buf, itoa(vehicle.lap_number), 112, 12, brightness);
            }
        }else{
            if (vehicle.lap_number == TOTAL_LAPS + 1){
                draw_text(tx_buf, "#", 100, 12, brightness);
            }else{
                draw_text(tx_buf, itoa(vehicle.lap_number), 100, 12, brightness);
            }
        }
        
        draw_char(tx_buf, '/', 125, 12, brightness);
        draw_text(tx_buf, itoa(TOTAL_LAPS), 130, 12, brightness);
        
    // DRIVE MODE
//        if(VcuState_A.DRIVE==1){
//          draw_char(tx_buf, 'D', 59, 33, brightness);
//        }else if(VcuState_A.REVERSE==1){ 
//          draw_char(tx_buf, 'R', 59, 33, brightness);
//        }else{
//          draw_char(tx_buf, 'N', 59, 33, brightness);
//        }
           
        
    // TIME    
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "T", 0, 27, brightness);
        select_font(&FreeSans9pt7b);
        draw_text(tx_buf, itoa(vehicle.lap_sec), 8, 31, brightness);       
              
    // DELTA TIME
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "dT", 78, 27, brightness);
        select_font(&FreeSans9pt7b);
        if(vehicle.delta_time_sec < 0){
            draw_text(tx_buf, "-", 91, 29, brightness);
            draw_text(tx_buf, itoa(vehicle.delta_time_sec*(-1)), 100, 31, brightness);
        }else if(vehicle.delta_time_sec == 0){
            draw_text(tx_buf, itoa(vehicle.delta_time_sec), 91, 31, brightness);
        }else{
            draw_text(tx_buf, "+", 91, 29, brightness);
            draw_text(tx_buf, itoa(vehicle.delta_time_sec), 100, 31, brightness);
        }
      
        
    // CAN message count
//        select_font(&Font5x7FixedMono);
//        draw_text(tx_buf, "MSGS:", 0, 45, brightness);
//        draw_text(tx_buf, itoa(can_msg_num), 30, 45, brightness);
        
    // LAST LAP JOULE
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "PREVJ:", 0, 45, brightness);
        if (vehicle.lap_number == 0) {
            draw_text(tx_buf, itoa(vehicle.lap_joules[0]), 37, 45, brightness);
        } else {
            draw_text(tx_buf, itoa(vehicle.lap_joules[vehicle.lap_number-1]), 30, 45, brightness);
        }
        
    // LAP JOULE
        select_font(&Font5x7FixedMono);
        draw_text(tx_buf, "LAPJ:", 78, 46, brightness);
        draw_text(tx_buf, itoa(vehicle.lap_joule), 109, 46, brightness);  
        
    // MISCELLANEOUS
        select_font(&Org_01);
        
        //display brightness
        if(flags.adaptive_brightness_mode){
            draw_pixel(tx_buf, 2, 49, brightness);//brightness symbol
            draw_pixel(tx_buf, 3, 49, brightness);
            draw_pixel(tx_buf, 4, 49, brightness);
            draw_pixel(tx_buf, 2, 50, brightness);
            draw_pixel(tx_buf, 3, 50, brightness);
            draw_pixel(tx_buf, 4, 50, brightness);
            draw_pixel(tx_buf, 2, 51, brightness);
            draw_pixel(tx_buf, 3, 51, brightness);
            draw_pixel(tx_buf, 4, 51, brightness);
            draw_pixel(tx_buf, 0, 50, brightness);
            draw_pixel(tx_buf, 1, 50, brightness);
            draw_pixel(tx_buf, 3, 47, brightness);
            draw_pixel(tx_buf, 3, 48, brightness);
            draw_pixel(tx_buf, 5, 50, brightness);
            draw_pixel(tx_buf, 6, 50, brightness);
            draw_pixel(tx_buf, 3, 52, brightness);
            draw_pixel(tx_buf, 3, 53, brightness);
            draw_pixel(tx_buf, 0, 47, brightness);
            draw_pixel(tx_buf, 1, 48, brightness);
            draw_pixel(tx_buf, 0, 53, brightness);
            draw_pixel(tx_buf, 1, 52, brightness);
            draw_pixel(tx_buf, 5, 48, brightness);
            draw_pixel(tx_buf, 6, 47, brightness);
            draw_pixel(tx_buf, 5, 52, brightness);
            draw_pixel(tx_buf, 6, 53, brightness);
            
            draw_text(tx_buf, itoa(adaptive_brightness), 10, 52, brightness);
        }
        
        //voltage
//        draw_text(tx_buf, itoa(vehicle.voltage), 0, 60, brightness);
//        draw_text(tx_buf, ".", 12, 60, brightness);
//        draw_text(tx_buf, itoa((vehicle.voltage-(uint8_t)vehicle.voltage)*10), 14, 60, brightness);
//        draw_text(tx_buf, "V", 21, 60, brightness);
        
        //distance in meters
        draw_text(tx_buf, "DIST:", 30, 60, brightness);
        draw_text(tx_buf, itoa(vehicle.distance), 57, 60, brightness);
        
        //display update time in milliseconds
        draw_text(tx_buf, "updT:", 87, 60, brightness);
        draw_text(tx_buf, itoa(display_update_cnt), 112, 60, brightness);
        
        //display refresh frequency
        draw_text(tx_buf, itoa(display_hz), 130, 60, brightness);
        draw_text(tx_buf, "Hz", 143, 60, brightness);
        
        
        
        
//        for(int i=0;i<16;i++){
//            draw_pixel(tx_buf, i+135, 54, i);
//            draw_pixel(tx_buf, i+135, 55, i);
//            draw_pixel(tx_buf, i+135, 56, i);
//            draw_pixel(tx_buf, i+135, 57, i);
//            draw_pixel(tx_buf, i+135, 58, i);
//            draw_pixel(tx_buf, i+135, 59, i);
//            draw_pixel(tx_buf, i+135, 60, i);
//            draw_pixel(tx_buf, i+135, 61, i);
//            draw_pixel(tx_buf, i+135, 62, i);
//        }
        
          
        
        draw_vline(tx_buf, 155, 0, 63, brightness);
        
    // SPEED GRAPH   
        offset = vehicle.distance/LUT_DISTANCE_RESOLUTION; //because the speed resolution is more than 1 meter the distance must be divided by this number
        
        SpeedArrow(vehicle.speed*LUT_DISTANCE_RESOLUTION, brightness);
        for(int i=offset;i<offset+86;i++){ //the offset value scrolls the speed graph, thus the scrolling speed is dependent on the vehicle speed
            if(i+170-offset>=170){
                draw_pixel(tx_buf, i+170-offset, (lut[i]*(-1)+SPD_GRAPH_OFFSET), brightness);
            }
        }
        
    // ACC CHEVRON 
        if(vehicle.lap_number>0){
        if(lut[(int)offset] > vehicle.speed*2) AccChevron(56, 17);
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

void GoToSleep(void){
    //add lines if necessary
    //RCONbits.SLEEP = 0;
    RCONbits.IDLE = 0;
    RCONbits.WDTO = 0;
    //Disable DMA interrupts
    IPC1bits.DMA0IP = 0;
    IPC6bits.DMA2IP = 0;
    //ADC disabled in idle ADSIDL = 1
    Idle();   
}

void ReturnFromSleep(void){
    if(RCONbits.IDLE == 1){
        RCONbits.IDLE = 0;
        if(RCONbits.WDTO == 1){
            RCONbits.WDTO = 0;
        }
        ClrWdt();
        IPC1bits.DMA0IP = 1;
        IPC6bits.DMA2IP = 1;
    }
    //add lines if necessary
}






