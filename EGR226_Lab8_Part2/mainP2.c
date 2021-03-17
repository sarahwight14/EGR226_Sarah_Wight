/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/17/2021
* Project: Lab 8 Part 2
* File: main.c
* Description: This program uses Timer A to generate a PWM signal that
* is connected directly to an I/O pin for controlling the speed of a DC motor.
**************************************************************************************/


#include "msp.h"

void timerA_Init(void);             // Declaring custom functions for later use
void SysTick_Init(void);
void timerA(uint16_t duty);
void SysTick_delay(uint16_t delay);

volatile float dutyCycle = 20;      // Global variable to be adjusted while code is running
volatile float period = 37500;      // Period to attain 40 Hz frequency

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // Stop watchdog timer

    float sendDutyCycle;     // Declaring variable to be sent to Timer A function

    SysTick_Init();          // Calling initialization function for systick timer
    timerA_Init();           // Calling initialization function for Timer A

    while(1)
    {
        sendDutyCycle = ((dutyCycle/100) * period);      // Calculation for time on

        timerA((int)sendDutyCycle);                      // Typecasting time on to int to fit parameters of Timer A function
        SysTick_delay(50);                               // Delay
    }
}

/****| timerA_Init | *****************************************
* Brief: This function is used for the initialization
* of Timer A
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void timerA_Init(void)
{
    // Configure GPIO for PWM output
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~(BIT4);
    P2->DIR |= BIT4;
}

/****| timerA | *****************************************
* Brief: This function sets the sets the period, mode,
* duty cycle, and register of timer A
* param:
* (uint16_t) data: one integer worth of data input that
* determines the duty cycle
* return:
* N/A
*************************************************************/
void timerA(uint16_t dc)
{
    TIMER_A0->CCR[0] = period - 1;                  // PWM period (# cycles of clock)
    TIMER_A0->CCTL[1] = 0xE0;                       // CCR1 reset/set mode 7
    TIMER_A0->CCR[1] = dc;                          // CCR1 PWM duty cycle
    TIMER_A0->CTL = 0x0214;                         // SMCLK, Up mode, clear TAR to start
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
    SysTick->CTRL = 0;            // Disable SysTick
    SysTick->LOAD = 0x00FFFFFF;   // Max reload value
    SysTick->VAL = 0;             // Any write to current clears it
    SysTick->CTRL = 0x00000005;   // Enable systick, 3MHz, no interrupts
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

