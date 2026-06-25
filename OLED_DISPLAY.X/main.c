/**
  Section: Included Files
*/
#include "user.h"
#include "SSD1322_API.h"
#include "SSD1322_GFX.h"
#include "SSD1322_HW_Driver.h"
/*
                         Main application
 */





int main(void)
{
    
    SYSTEM_Initialize();
    
    
    CAN_STB_SetLow();
    CAN1_Initialize();
    
    TMR2_Start();//delay timer

    SSD1322_API_init();
    set_buffer_size(256, 64);
//    CAN1_TransmitEnable();
    CAN1_ReceiveEnable();
    LED_LG_SetHigh();
    while(CAN_OP_MODE_REQUEST_FAIL == CAN1_OperationModeSet(CAN_CONFIGURATION_MODE));
    LED_LG_SetLow();
    fill_buffer(tx_buf, 0);
    send_buffer_to_OLED(tx_buf, 0, 0);
    CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);
    
    
    
    while (1){
        
//        if(CAN1_ReceivedMessageCountGet()){
//            CAN_Receive();
//        }
        if(flags.can_message_received){
            flags.can_message_received = false;
            can_msg_num++;
            CAN_Receive();
        }
        
        if(BUTTON_L_GetValue() && !flags.debounce){
            flags.debounce = true;
            flags.adaptive_brightness_mode = !flags.adaptive_brightness_mode; //toggle flag
        }
        
        if(flags.update_display == true){
            flags.update_display = false;
            if(flags.adaptive_brightness_mode){ //make sure that the phototransistor resistors are 1Mohm for this to work properly
                CalculateDisplayValues();
                tmr1_cnt = 0;
                UpdateDisplay(adaptive_brightness);
                prev_adaptive_brightness = adaptive_brightness;
                display_update_cnt = tmr1_cnt;  
                GetBrightnessADC();
            }else{
                CalculateDisplayValues();
                tmr1_cnt = 0;
                UpdateDisplay(DISPLAY_BRIGHTNESS);
                display_update_cnt = tmr1_cnt;  
            } 
        }
        GoToSleep();
        ReturnFromSleep();
    }
    return 0;
}
/**
 End of File
*/
