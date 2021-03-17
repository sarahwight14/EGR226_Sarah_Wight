/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/17/2021
* Project: Lab 8 Part 1
* File: main.c
* Description: This program uses the SysTick timer to generate the PWM mode to create
* an analog signal for controlling the speed of a DC motor
**************************************************************************************/


#include "msp.h"

void SysTick_Init(void);                // Declaring custom functions for later use
void SysTick_delay(uint16_t delay);

volatile float dutyCycle;       // Global variable to be adjusted while code is running

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // Stop watchdog timer

    float period, onTime, offTime;                  // Declaring variables for later use
    int delayOn, delayOff;

    dutyCycle = 10;         // initial duty cycle setup, will be changed while running
    period = 25;            // Period (in ms) to attain 40 Hz frequency

    SysTick_Init();         // Calling initialization function for systick timer

    P2->SEL0 &= ~BIT4;      // Configure P2.4 as simple I/O
    P2->SEL1 &= ~BIT4;
    P2->DIR |= BIT4;        // P2.4 set as output

    while (1)
    {
        onTime = ((dutyCycle/100) * period);     // Calculation for time on
        offTime = period - onTime;               // Calculation for time off

        delayOn = (int)onTime + 1;               // Typecast to int to fit parameters of Systick delay function
        delayOff = (int)offTime + 1;

        P2->OUT |= BIT4;                         // Turn motor on
        SysTick_delay(delayOn);                  // Delays for time on

        P2->OUT &= ~BIT4;                        // Turn motor off
        SysTick_delay(delayOff);                 // Delays for time off
    }
}

/****| SysTick_Init | *****************************************
* Brief: This function is used for the initialization
* of the SysTick timer
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void SysTick_Init(void)
{
    SysTick -> CTRL = 0;            // Disable SysTick
    SysTick -> LOAD = 0x00FFFFFF;   // Max reload value
    SysTick -> VAL = 0;             // Any write to current clears it
    SysTick -> CTRL = 0x00000005;   // Enable systick, 3MHz, no interrupts
}

/****| SysTick_delay | *****************************************
* Brief: This function sets the SysTick timer to be used as a
* delay when called
* param:
* (uint16_t) data: one integer worth of data input that
* determines the duration of the delay
* return:
* N/A
*************************************************************/
void SysTick_delay(uint16_t delay)
{
    SysTick->LOAD = ((delay * 3000) - 1);       // Delay 1 msecond per delay value
    SysTick->VAL = 0;                           // Any write to CVR clears it
    while ((SysTick->CTRL & 0x00010000) == 0);  // Wait for flag to be SET
}
