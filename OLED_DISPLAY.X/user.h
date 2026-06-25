#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>
#include "user_defines.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/tmr2.h"
#include "SSD1322_GFX.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/can1.h"
#include "mcc_generated_files/adc1.h"



struct VEHICLE{
    double rpm;
    bool reverse;
    double speed;
    double distance;
    double prev_distance;
    double joule;
    double lap_joule;
    float lap_joules[255];
    double best_lap_joule;
    double total_joule;
    double voltage;
    uint8_t lap_number;
    uint16_t lap_ms;
    uint16_t lap_sec;
    uint16_t previous_lap_sec;
    int16_t delta_time_sec;
    
};

typedef union __attribute__((packed)) 
{
    uint16_t Word;
    struct
    {
        uint8_t LowByte     :8;
        uint8_t HighByte    :8;
    };
}CAN_Bytes;

typedef union  __attribute__((packed))
{
    uint8_t bits;
    struct
    {
        uint8_t LIGHTS_DRL      :1;
        uint8_t LIGHTS_HAZARD   :1;
        uint8_t AUTONOMOUS      :1;
        uint8_t BRAKE           :1;
        uint8_t LIGHTS_ENABLE   :1;
        uint8_t MC_OW           :1;
        uint8_t WIPER           :1;
        uint8_t PESC_SLEEP      :1;
    };
} VCU_STATE_A;

typedef union  __attribute__((packed))
{
    uint8_t bits;
    struct
    {
        uint8_t ACC        :1;
        uint8_t DRIVE      :1;
        uint8_t REVERSE    :1;
        uint8_t LAP        :1;
        uint8_t TS_L       :1;
        uint8_t TS_R       :1;
        uint8_t FUNCTION1  :1;
        uint8_t FUNCTION2  :1;
    };
} STW_STATE_BUTTONS;

struct FLAGS{
    bool update_synced;
    bool update_display;
    bool debounce;
    bool adaptive_brightness_mode;
    bool can_message_received;
};

extern volatile struct FLAGS flags;

extern volatile struct VEHICLE vehicle;
extern volatile CAN_Bytes rpm;
extern volatile CAN_Bytes bms;
extern volatile VCU_STATE_A VcuState_A;
//extern volatile VCU_STATE_B VcuState_B;
extern volatile STW_STATE_BUTTONS StwState;

//ADC
extern ADC1_CHANNEL left_phototrans;
extern ADC1_CHANNEL right_phototrans;
extern uint16_t left_brightness;
extern uint16_t right_brightness;
extern uint16_t adaptive_brightness;
extern uint16_t prev_adaptive_brightness;

extern volatile uint8_t SPI_data[8];


extern uint8_t tx_buf[256 * 64 / 2];
extern double offset;
extern int display_update_cnt;


extern uint8_t lut[642];
extern uint16_t lut_size;

extern uint32_t tmr1_cnt;
extern bool tmr1_flag;

extern uint32_t tmr1_1s_cnt;
extern bool tmr1_1s_flag;
extern uint32_t display_hz_cnt;
extern uint32_t display_hz;

extern uint16_t update_cnt_100ms;
extern uint16_t debounce_500ms;

extern uint32_t cnt;
extern bool cnt_flag;

extern uint16_t can_msg_num;


char* itoa(uint32_t value);
void UpdateDisplay(uint8_t brightness);
void CAN_Receive(void);
void CAN_Transmit(void);
void CalculateDisplayValues(void);
void GetBrightnessADC(void);
void SpeedArrow(double double_speed, uint8_t brightness);
void AccChevron(uint16_t x_pos, uint16_t y_pos);
void GoToSleep(void);
void ReturnFromSleep(void);


#endif