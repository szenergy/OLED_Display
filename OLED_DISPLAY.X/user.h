#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>
#include "user_defines.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "SSD1322_GFX.h"
#include "SSD1322_API.h"
#include "mcc_generated_files/can1.h"



struct VEHICLE{
    float rpm;
    bool reverse;
    float speed;
    float distance;
    float prev_distance;
    float total_joule;
    float lap_joule;
    float prev_lap_joule;
    float voltage;
    float current;
    uint8_t lap_number;
    float lap_sec;
    float delta_time_sec;
    
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
        uint8_t RESET      :1;
        uint8_t FN1        :1;
    };
} STW_STATE_BUTTONS;

typedef enum {
	ROT_1 = 0,
	ROT_2 = 1,
	ROT_3 = 2,
	ROT_4 = 4,
	ROT_5 = 8,
	ROT_6 = 16,
	ROT_7 = 32,
	ROT_8 = 64
} ROT_POS_ENUM;

struct FLAGS{
    bool update_display;
    bool can_message_received;
};

typedef enum {
    UD_TOP_LEFT = 1,
    UD_TOP_RIGHT,
    UD_BOTTOM_LEFT,
    UD_BOTTOM_RIGHT
} USR_DISPLAY_ALIGNMENT;

extern volatile struct FLAGS flags;

extern volatile struct VEHICLE vehicle;
extern volatile VCU_STATE_A VCU_A;
extern volatile STW_STATE_BUTTONS Steering_Wheel;

extern uint8_t tx_buf[256 * 64 / 2];

extern bool display_off;


char* itoa(uint32_t value);
int32_t map_value(int32_t x, int32_t min_x, int32_t max_x, int32_t min_to, int32_t max_to);

void UpdateDisplay();
void CAN_Receive(void);
void CalculateDisplayValues(void);
void User_Idle_Normal(void);
void Handle_Display_Off();


#endif