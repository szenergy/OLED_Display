/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.169.0
        Device            :  dsPIC33EV256GM106
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.50
        MPLAB 	          :  MPLAB X v5.40
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H
/**
    Section: Includes
*/
#include <xc.h>

/**
    Section: Device Pin Macros
*/
/**
  @Summary
    Sets the GPIO pin, RA10, high using LATA10.

  @Description
    Sets the GPIO pin, RA10, high using LATA10.

  @Preconditions
    The RA10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA10 high (1)
    OLED_DC_SetHigh();
    </code>

*/
#define OLED_DC_SetHigh()          (_LATA10 = 1)
/**
  @Summary
    Sets the GPIO pin, RA10, low using LATA10.

  @Description
    Sets the GPIO pin, RA10, low using LATA10.

  @Preconditions
    The RA10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA10 low (0)
    OLED_DC_SetLow();
    </code>

*/
#define OLED_DC_SetLow()           (_LATA10 = 0)
/**
  @Summary
    Toggles the GPIO pin, RA10, using LATA10.

  @Description
    Toggles the GPIO pin, RA10, using LATA10.

  @Preconditions
    The RA10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA10
    OLED_DC_Toggle();
    </code>

*/
#define OLED_DC_Toggle()           (_LATA10 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RA10.

  @Description
    Reads the value of the GPIO pin, RA10.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA10
    postValue = OLED_DC_GetValue();
    </code>

*/
#define OLED_DC_GetValue()         _RA10
/**
  @Summary
    Configures the GPIO pin, RA10, as an input.

  @Description
    Configures the GPIO pin, RA10, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA10 as an input
    OLED_DC_SetDigitalInput();
    </code>

*/
#define OLED_DC_SetDigitalInput()  (_TRISA10 = 1)
/**
  @Summary
    Configures the GPIO pin, RA10, as an output.

  @Description
    Configures the GPIO pin, RA10, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA10 as an output
    OLED_DC_SetDigitalOutput();
    </code>

*/
#define OLED_DC_SetDigitalOutput() (_TRISA10 = 0)
/**
  @Summary
    Sets the GPIO pin, RA4, high using LATA4.

  @Description
    Sets the GPIO pin, RA4, high using LATA4.

  @Preconditions
    The RA4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA4 high (1)
    PIN_33_SetHigh();
    </code>

*/
#define PIN_33_SetHigh()          (_LATA4 = 1)
/**
  @Summary
    Sets the GPIO pin, RA4, low using LATA4.

  @Description
    Sets the GPIO pin, RA4, low using LATA4.

  @Preconditions
    The RA4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA4 low (0)
    PIN_33_SetLow();
    </code>

*/
#define PIN_33_SetLow()           (_LATA4 = 0)
/**
  @Summary
    Toggles the GPIO pin, RA4, using LATA4.

  @Description
    Toggles the GPIO pin, RA4, using LATA4.

  @Preconditions
    The RA4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA4
    PIN_33_Toggle();
    </code>

*/
#define PIN_33_Toggle()           (_LATA4 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RA4.

  @Description
    Reads the value of the GPIO pin, RA4.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA4
    postValue = PIN_33_GetValue();
    </code>

*/
#define PIN_33_GetValue()         _RA4
/**
  @Summary
    Configures the GPIO pin, RA4, as an input.

  @Description
    Configures the GPIO pin, RA4, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA4 as an input
    PIN_33_SetDigitalInput();
    </code>

*/
#define PIN_33_SetDigitalInput()  (_TRISA4 = 1)
/**
  @Summary
    Configures the GPIO pin, RA4, as an output.

  @Description
    Configures the GPIO pin, RA4, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA4 as an output
    PIN_33_SetDigitalOutput();
    </code>

*/
#define PIN_33_SetDigitalOutput() (_TRISA4 = 0)
/**
  @Summary
    Sets the GPIO pin, RA8, high using LATA8.

  @Description
    Sets the GPIO pin, RA8, high using LATA8.

  @Preconditions
    The RA8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA8 high (1)
    BUTTON_L_SetHigh();
    </code>

*/
#define BUTTON_L_SetHigh()          (_LATA8 = 1)
/**
  @Summary
    Sets the GPIO pin, RA8, low using LATA8.

  @Description
    Sets the GPIO pin, RA8, low using LATA8.

  @Preconditions
    The RA8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA8 low (0)
    BUTTON_L_SetLow();
    </code>

*/
#define BUTTON_L_SetLow()           (_LATA8 = 0)
/**
  @Summary
    Toggles the GPIO pin, RA8, using LATA8.

  @Description
    Toggles the GPIO pin, RA8, using LATA8.

  @Preconditions
    The RA8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA8
    BUTTON_L_Toggle();
    </code>

*/
#define BUTTON_L_Toggle()           (_LATA8 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RA8.

  @Description
    Reads the value of the GPIO pin, RA8.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA8
    postValue = BUTTON_L_GetValue();
    </code>

*/
#define BUTTON_L_GetValue()         _RA8
/**
  @Summary
    Configures the GPIO pin, RA8, as an input.

  @Description
    Configures the GPIO pin, RA8, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA8 as an input
    BUTTON_L_SetDigitalInput();
    </code>

*/
#define BUTTON_L_SetDigitalInput()  (_TRISA8 = 1)
/**
  @Summary
    Configures the GPIO pin, RA8, as an output.

  @Description
    Configures the GPIO pin, RA8, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA8 as an output
    BUTTON_L_SetDigitalOutput();
    </code>

*/
#define BUTTON_L_SetDigitalOutput() (_TRISA8 = 0)
/**
  @Summary
    Sets the GPIO pin, RB12, high using LATB12.

  @Description
    Sets the GPIO pin, RB12, high using LATB12.

  @Preconditions
    The RB12 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB12 high (1)
    OLED_CS_SetHigh();
    </code>

*/
#define OLED_CS_SetHigh()          (_LATB12 = 1)
/**
  @Summary
    Sets the GPIO pin, RB12, low using LATB12.

  @Description
    Sets the GPIO pin, RB12, low using LATB12.

  @Preconditions
    The RB12 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB12 low (0)
    OLED_CS_SetLow();
    </code>

*/
#define OLED_CS_SetLow()           (_LATB12 = 0)
/**
  @Summary
    Toggles the GPIO pin, RB12, using LATB12.

  @Description
    Toggles the GPIO pin, RB12, using LATB12.

  @Preconditions
    The RB12 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB12
    OLED_CS_Toggle();
    </code>

*/
#define OLED_CS_Toggle()           (_LATB12 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RB12.

  @Description
    Reads the value of the GPIO pin, RB12.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB12
    postValue = OLED_CS_GetValue();
    </code>

*/
#define OLED_CS_GetValue()         _RB12
/**
  @Summary
    Configures the GPIO pin, RB12, as an input.

  @Description
    Configures the GPIO pin, RB12, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB12 as an input
    OLED_CS_SetDigitalInput();
    </code>

*/
#define OLED_CS_SetDigitalInput()  (_TRISB12 = 1)
/**
  @Summary
    Configures the GPIO pin, RB12, as an output.

  @Description
    Configures the GPIO pin, RB12, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB12 as an output
    OLED_CS_SetDigitalOutput();
    </code>

*/
#define OLED_CS_SetDigitalOutput() (_TRISB12 = 0)
/**
  @Summary
    Sets the GPIO pin, RB13, high using LATB13.

  @Description
    Sets the GPIO pin, RB13, high using LATB13.

  @Preconditions
    The RB13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB13 high (1)
    OLED_RESET_SetHigh();
    </code>

*/
#define OLED_RESET_SetHigh()          (_LATB13 = 1)
/**
  @Summary
    Sets the GPIO pin, RB13, low using LATB13.

  @Description
    Sets the GPIO pin, RB13, low using LATB13.

  @Preconditions
    The RB13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB13 low (0)
    OLED_RESET_SetLow();
    </code>

*/
#define OLED_RESET_SetLow()           (_LATB13 = 0)
/**
  @Summary
    Toggles the GPIO pin, RB13, using LATB13.

  @Description
    Toggles the GPIO pin, RB13, using LATB13.

  @Preconditions
    The RB13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB13
    OLED_RESET_Toggle();
    </code>

*/
#define OLED_RESET_Toggle()           (_LATB13 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RB13.

  @Description
    Reads the value of the GPIO pin, RB13.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB13
    postValue = OLED_RESET_GetValue();
    </code>

*/
#define OLED_RESET_GetValue()         _RB13
/**
  @Summary
    Configures the GPIO pin, RB13, as an input.

  @Description
    Configures the GPIO pin, RB13, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB13 as an input
    OLED_RESET_SetDigitalInput();
    </code>

*/
#define OLED_RESET_SetDigitalInput()  (_TRISB13 = 1)
/**
  @Summary
    Configures the GPIO pin, RB13, as an output.

  @Description
    Configures the GPIO pin, RB13, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB13 as an output
    OLED_RESET_SetDigitalOutput();
    </code>

*/
#define OLED_RESET_SetDigitalOutput() (_TRISB13 = 0)
/**
  @Summary
    Sets the GPIO pin, RB2, high using LATB2.

  @Description
    Sets the GPIO pin, RB2, high using LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB2 high (1)
    LED_LY_SetHigh();
    </code>

*/
#define LED_LY_SetHigh()          (_LATB2 = 1)
/**
  @Summary
    Sets the GPIO pin, RB2, low using LATB2.

  @Description
    Sets the GPIO pin, RB2, low using LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB2 low (0)
    LED_LY_SetLow();
    </code>

*/
#define LED_LY_SetLow()           (_LATB2 = 0)
/**
  @Summary
    Toggles the GPIO pin, RB2, using LATB2.

  @Description
    Toggles the GPIO pin, RB2, using LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB2
    LED_LY_Toggle();
    </code>

*/
#define LED_LY_Toggle()           (_LATB2 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RB2.

  @Description
    Reads the value of the GPIO pin, RB2.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB2
    postValue = LED_LY_GetValue();
    </code>

*/
#define LED_LY_GetValue()         _RB2
/**
  @Summary
    Configures the GPIO pin, RB2, as an input.

  @Description
    Configures the GPIO pin, RB2, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB2 as an input
    LED_LY_SetDigitalInput();
    </code>

*/
#define LED_LY_SetDigitalInput()  (_TRISB2 = 1)
/**
  @Summary
    Configures the GPIO pin, RB2, as an output.

  @Description
    Configures the GPIO pin, RB2, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB2 as an output
    LED_LY_SetDigitalOutput();
    </code>

*/
#define LED_LY_SetDigitalOutput() (_TRISB2 = 0)
/**
  @Summary
    Sets the GPIO pin, RB3, high using LATB3.

  @Description
    Sets the GPIO pin, RB3, high using LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB3 high (1)
    LED_LR_SetHigh();
    </code>

*/
#define LED_LR_SetHigh()          (_LATB3 = 1)
/**
  @Summary
    Sets the GPIO pin, RB3, low using LATB3.

  @Description
    Sets the GPIO pin, RB3, low using LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB3 low (0)
    LED_LR_SetLow();
    </code>

*/
#define LED_LR_SetLow()           (_LATB3 = 0)
/**
  @Summary
    Toggles the GPIO pin, RB3, using LATB3.

  @Description
    Toggles the GPIO pin, RB3, using LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB3
    LED_LR_Toggle();
    </code>

*/
#define LED_LR_Toggle()           (_LATB3 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RB3.

  @Description
    Reads the value of the GPIO pin, RB3.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB3
    postValue = LED_LR_GetValue();
    </code>

*/
#define LED_LR_GetValue()         _RB3
/**
  @Summary
    Configures the GPIO pin, RB3, as an input.

  @Description
    Configures the GPIO pin, RB3, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB3 as an input
    LED_LR_SetDigitalInput();
    </code>

*/
#define LED_LR_SetDigitalInput()  (_TRISB3 = 1)
/**
  @Summary
    Configures the GPIO pin, RB3, as an output.

  @Description
    Configures the GPIO pin, RB3, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB3 as an output
    LED_LR_SetDigitalOutput();
    </code>

*/
#define LED_LR_SetDigitalOutput() (_TRISB3 = 0)
/**
  @Summary
    Sets the GPIO pin, RB8, high using LATB8.

  @Description
    Sets the GPIO pin, RB8, high using LATB8.

  @Preconditions
    The RB8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB8 high (1)
    channel_AN26_SetHigh();
    </code>

*/
#define channel_AN26_SetHigh()          (_LATB8 = 1)
/**
  @Summary
    Sets the GPIO pin, RB8, low using LATB8.

  @Description
    Sets the GPIO pin, RB8, low using LATB8.

  @Preconditions
    The RB8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB8 low (0)
    channel_AN26_SetLow();
    </code>

*/
#define channel_AN26_SetLow()           (_LATB8 = 0)
/**
  @Summary
    Toggles the GPIO pin, RB8, using LATB8.

  @Description
    Toggles the GPIO pin, RB8, using LATB8.

  @Preconditions
    The RB8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB8
    channel_AN26_Toggle();
    </code>

*/
#define channel_AN26_Toggle()           (_LATB8 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RB8.

  @Description
    Reads the value of the GPIO pin, RB8.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB8
    postValue = channel_AN26_GetValue();
    </code>

*/
#define channel_AN26_GetValue()         _RB8
/**
  @Summary
    Configures the GPIO pin, RB8, as an input.

  @Description
    Configures the GPIO pin, RB8, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB8 as an input
    channel_AN26_SetDigitalInput();
    </code>

*/
#define channel_AN26_SetDigitalInput()  (_TRISB8 = 1)
/**
  @Summary
    Configures the GPIO pin, RB8, as an output.

  @Description
    Configures the GPIO pin, RB8, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB8 as an output
    channel_AN26_SetDigitalOutput();
    </code>

*/
#define channel_AN26_SetDigitalOutput() (_TRISB8 = 0)
/**
  @Summary
    Sets the GPIO pin, RB9, high using LATB9.

  @Description
    Sets the GPIO pin, RB9, high using LATB9.

  @Preconditions
    The RB9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB9 high (1)
    LED_RR_SetHigh();
    </code>

*/
#define LED_RR_SetHigh()          (_LATB9 = 1)
/**
  @Summary
    Sets the GPIO pin, RB9, low using LATB9.

  @Description
    Sets the GPIO pin, RB9, low using LATB9.

  @Preconditions
    The RB9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB9 low (0)
    LED_RR_SetLow();
    </code>

*/
#define LED_RR_SetLow()           (_LATB9 = 0)
/**
  @Summary
    Toggles the GPIO pin, RB9, using LATB9.

  @Description
    Toggles the GPIO pin, RB9, using LATB9.

  @Preconditions
    The RB9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB9
    LED_RR_Toggle();
    </code>

*/
#define LED_RR_Toggle()           (_LATB9 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RB9.

  @Description
    Reads the value of the GPIO pin, RB9.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB9
    postValue = LED_RR_GetValue();
    </code>

*/
#define LED_RR_GetValue()         _RB9
/**
  @Summary
    Configures the GPIO pin, RB9, as an input.

  @Description
    Configures the GPIO pin, RB9, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB9 as an input
    LED_RR_SetDigitalInput();
    </code>

*/
#define LED_RR_SetDigitalInput()  (_TRISB9 = 1)
/**
  @Summary
    Configures the GPIO pin, RB9, as an output.

  @Description
    Configures the GPIO pin, RB9, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB9 as an output
    LED_RR_SetDigitalOutput();
    </code>

*/
#define LED_RR_SetDigitalOutput() (_TRISB9 = 0)
/**
  @Summary
    Sets the GPIO pin, RC0, high using LATC0.

  @Description
    Sets the GPIO pin, RC0, high using LATC0.

  @Preconditions
    The RC0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC0 high (1)
    LED_LG_SetHigh();
    </code>

*/
#define LED_LG_SetHigh()          (_LATC0 = 1)
/**
  @Summary
    Sets the GPIO pin, RC0, low using LATC0.

  @Description
    Sets the GPIO pin, RC0, low using LATC0.

  @Preconditions
    The RC0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC0 low (0)
    LED_LG_SetLow();
    </code>

*/
#define LED_LG_SetLow()           (_LATC0 = 0)
/**
  @Summary
    Toggles the GPIO pin, RC0, using LATC0.

  @Description
    Toggles the GPIO pin, RC0, using LATC0.

  @Preconditions
    The RC0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC0
    LED_LG_Toggle();
    </code>

*/
#define LED_LG_Toggle()           (_LATC0 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RC0.

  @Description
    Reads the value of the GPIO pin, RC0.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC0
    postValue = LED_LG_GetValue();
    </code>

*/
#define LED_LG_GetValue()         _RC0
/**
  @Summary
    Configures the GPIO pin, RC0, as an input.

  @Description
    Configures the GPIO pin, RC0, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC0 as an input
    LED_LG_SetDigitalInput();
    </code>

*/
#define LED_LG_SetDigitalInput()  (_TRISC0 = 1)
/**
  @Summary
    Configures the GPIO pin, RC0, as an output.

  @Description
    Configures the GPIO pin, RC0, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC0 as an output
    LED_LG_SetDigitalOutput();
    </code>

*/
#define LED_LG_SetDigitalOutput() (_TRISC0 = 0)
/**
  @Summary
    Sets the GPIO pin, RC10, high using LATC10.

  @Description
    Sets the GPIO pin, RC10, high using LATC10.

  @Preconditions
    The RC10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC10 high (1)
    CAN_STB_SetHigh();
    </code>

*/
#define CAN_STB_SetHigh()          (_LATC10 = 1)
/**
  @Summary
    Sets the GPIO pin, RC10, low using LATC10.

  @Description
    Sets the GPIO pin, RC10, low using LATC10.

  @Preconditions
    The RC10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC10 low (0)
    CAN_STB_SetLow();
    </code>

*/
#define CAN_STB_SetLow()           (_LATC10 = 0)
/**
  @Summary
    Toggles the GPIO pin, RC10, using LATC10.

  @Description
    Toggles the GPIO pin, RC10, using LATC10.

  @Preconditions
    The RC10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC10
    CAN_STB_Toggle();
    </code>

*/
#define CAN_STB_Toggle()           (_LATC10 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RC10.

  @Description
    Reads the value of the GPIO pin, RC10.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC10
    postValue = CAN_STB_GetValue();
    </code>

*/
#define CAN_STB_GetValue()         _RC10
/**
  @Summary
    Configures the GPIO pin, RC10, as an input.

  @Description
    Configures the GPIO pin, RC10, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC10 as an input
    CAN_STB_SetDigitalInput();
    </code>

*/
#define CAN_STB_SetDigitalInput()  (_TRISC10 = 1)
/**
  @Summary
    Configures the GPIO pin, RC10, as an output.

  @Description
    Configures the GPIO pin, RC10, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC10 as an output
    CAN_STB_SetDigitalOutput();
    </code>

*/
#define CAN_STB_SetDigitalOutput() (_TRISC10 = 0)
/**
  @Summary
    Sets the GPIO pin, RC13, high using LATC13.

  @Description
    Sets the GPIO pin, RC13, high using LATC13.

  @Preconditions
    The RC13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC13 high (1)
    BUTTON_R_SetHigh();
    </code>

*/
#define BUTTON_R_SetHigh()          (_LATC13 = 1)
/**
  @Summary
    Sets the GPIO pin, RC13, low using LATC13.

  @Description
    Sets the GPIO pin, RC13, low using LATC13.

  @Preconditions
    The RC13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC13 low (0)
    BUTTON_R_SetLow();
    </code>

*/
#define BUTTON_R_SetLow()           (_LATC13 = 0)
/**
  @Summary
    Toggles the GPIO pin, RC13, using LATC13.

  @Description
    Toggles the GPIO pin, RC13, using LATC13.

  @Preconditions
    The RC13 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC13
    BUTTON_R_Toggle();
    </code>

*/
#define BUTTON_R_Toggle()           (_LATC13 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RC13.

  @Description
    Reads the value of the GPIO pin, RC13.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC13
    postValue = BUTTON_R_GetValue();
    </code>

*/
#define BUTTON_R_GetValue()         _RC13
/**
  @Summary
    Configures the GPIO pin, RC13, as an input.

  @Description
    Configures the GPIO pin, RC13, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC13 as an input
    BUTTON_R_SetDigitalInput();
    </code>

*/
#define BUTTON_R_SetDigitalInput()  (_TRISC13 = 1)
/**
  @Summary
    Configures the GPIO pin, RC13, as an output.

  @Description
    Configures the GPIO pin, RC13, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC13 as an output
    BUTTON_R_SetDigitalOutput();
    </code>

*/
#define BUTTON_R_SetDigitalOutput() (_TRISC13 = 0)
/**
  @Summary
    Sets the GPIO pin, RC6, high using LATC6.

  @Description
    Sets the GPIO pin, RC6, high using LATC6.

  @Preconditions
    The RC6 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC6 high (1)
    LED_RY_SetHigh();
    </code>

*/
#define LED_RY_SetHigh()          (_LATC6 = 1)
/**
  @Summary
    Sets the GPIO pin, RC6, low using LATC6.

  @Description
    Sets the GPIO pin, RC6, low using LATC6.

  @Preconditions
    The RC6 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC6 low (0)
    LED_RY_SetLow();
    </code>

*/
#define LED_RY_SetLow()           (_LATC6 = 0)
/**
  @Summary
    Toggles the GPIO pin, RC6, using LATC6.

  @Description
    Toggles the GPIO pin, RC6, using LATC6.

  @Preconditions
    The RC6 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC6
    LED_RY_Toggle();
    </code>

*/
#define LED_RY_Toggle()           (_LATC6 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RC6.

  @Description
    Reads the value of the GPIO pin, RC6.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC6
    postValue = LED_RY_GetValue();
    </code>

*/
#define LED_RY_GetValue()         _RC6
/**
  @Summary
    Configures the GPIO pin, RC6, as an input.

  @Description
    Configures the GPIO pin, RC6, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC6 as an input
    LED_RY_SetDigitalInput();
    </code>

*/
#define LED_RY_SetDigitalInput()  (_TRISC6 = 1)
/**
  @Summary
    Configures the GPIO pin, RC6, as an output.

  @Description
    Configures the GPIO pin, RC6, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC6 as an output
    LED_RY_SetDigitalOutput();
    </code>

*/
#define LED_RY_SetDigitalOutput() (_TRISC6 = 0)
/**
  @Summary
    Sets the GPIO pin, RC7, high using LATC7.

  @Description
    Sets the GPIO pin, RC7, high using LATC7.

  @Preconditions
    The RC7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC7 high (1)
    LED_RG_SetHigh();
    </code>

*/
#define LED_RG_SetHigh()          (_LATC7 = 1)
/**
  @Summary
    Sets the GPIO pin, RC7, low using LATC7.

  @Description
    Sets the GPIO pin, RC7, low using LATC7.

  @Preconditions
    The RC7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC7 low (0)
    LED_RG_SetLow();
    </code>

*/
#define LED_RG_SetLow()           (_LATC7 = 0)
/**
  @Summary
    Toggles the GPIO pin, RC7, using LATC7.

  @Description
    Toggles the GPIO pin, RC7, using LATC7.

  @Preconditions
    The RC7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC7
    LED_RG_Toggle();
    </code>

*/
#define LED_RG_Toggle()           (_LATC7 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RC7.

  @Description
    Reads the value of the GPIO pin, RC7.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC7
    postValue = LED_RG_GetValue();
    </code>

*/
#define LED_RG_GetValue()         _RC7
/**
  @Summary
    Configures the GPIO pin, RC7, as an input.

  @Description
    Configures the GPIO pin, RC7, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC7 as an input
    LED_RG_SetDigitalInput();
    </code>

*/
#define LED_RG_SetDigitalInput()  (_TRISC7 = 1)
/**
  @Summary
    Configures the GPIO pin, RC7, as an output.

  @Description
    Configures the GPIO pin, RC7, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC7 as an output
    LED_RG_SetDigitalOutput();
    </code>

*/
#define LED_RG_SetDigitalOutput() (_TRISC7 = 0)
/**
  @Summary
    Sets the GPIO pin, RE15, high using LATE15.

  @Description
    Sets the GPIO pin, RE15, high using LATE15.

  @Preconditions
    The RE15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RE15 high (1)
    channel_AN15_SetHigh();
    </code>

*/
#define channel_AN15_SetHigh()          (_LATE15 = 1)
/**
  @Summary
    Sets the GPIO pin, RE15, low using LATE15.

  @Description
    Sets the GPIO pin, RE15, low using LATE15.

  @Preconditions
    The RE15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RE15 low (0)
    channel_AN15_SetLow();
    </code>

*/
#define channel_AN15_SetLow()           (_LATE15 = 0)
/**
  @Summary
    Toggles the GPIO pin, RE15, using LATE15.

  @Description
    Toggles the GPIO pin, RE15, using LATE15.

  @Preconditions
    The RE15 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RE15
    channel_AN15_Toggle();
    </code>

*/
#define channel_AN15_Toggle()           (_LATE15 ^= 1)
/**
  @Summary
    Reads the value of the GPIO pin, RE15.

  @Description
    Reads the value of the GPIO pin, RE15.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RE15
    postValue = channel_AN15_GetValue();
    </code>

*/
#define channel_AN15_GetValue()         _RE15
/**
  @Summary
    Configures the GPIO pin, RE15, as an input.

  @Description
    Configures the GPIO pin, RE15, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RE15 as an input
    channel_AN15_SetDigitalInput();
    </code>

*/
#define channel_AN15_SetDigitalInput()  (_TRISE15 = 1)
/**
  @Summary
    Configures the GPIO pin, RE15, as an output.

  @Description
    Configures the GPIO pin, RE15, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RE15 as an output
    channel_AN15_SetDigitalOutput();
    </code>

*/
#define channel_AN15_SetDigitalOutput() (_TRISE15 = 0)

/**
    Section: Function Prototypes
*/
/**
  @Summary
    Configures the pin settings of the dsPIC33EV256GM106
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        PIN_MANAGER_Initialize();
    }
    </code>

*/
void PIN_MANAGER_Initialize (void);



#endif
