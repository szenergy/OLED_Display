/**
  Section: Included Files
*/
#include "user.h"
#include "SSD1322_API.h"
#include "SSD1322_GFX.h"
#include "SSD1322_HW_Driver.h"
#include "mcc_generated_files/tmr1.h"
/*
        Main application
 */


int main(void)
{
    SYSTEM_Initialize();
    
    LED_LR_SetHigh();
    
    // init CAN
    CAN_STB_SetLow();
    CAN1_Initialize();
    CAN1_ReceiveEnable();
    while(CAN_OP_MODE_REQUEST_FAIL == CAN1_OperationModeSet(CAN_CONFIGURATION_MODE));
    CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);
    
//    SSD1322_HW_msDelay(100);
    
    // init oled display
    SSD1322_API_init();
    set_buffer_size(256, 64);
    fill_buffer(tx_buf, 0);
    send_buffer_to_OLED(tx_buf, 0, 0);
    
    display_off = false;
    Handle_Display_Off();
    
    TMR1_Start();
    
    LED_LR_SetLow();
    
    while (1){
        
        if(flags.can_message_received){
            flags.can_message_received = false;
            CAN_Receive();
//            LED_RG_Toggle();
        }
        
        if(flags.update_display){
            flags.update_display = false;
//            CalculateDisplayValues();
            UpdateDisplay();
//            LED_RY_Toggle();
        }
        
//        LED_LY_Toggle();
        
        User_Idle_Normal();
    }
    
    return 0;
}
/**
 End of File
*/
