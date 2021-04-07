/**************************************************************************************
 * Author: Sarah Wight
 * Course: EGR 226 - 902
 * Date: 4/7/2021
 * Project: Lab 11 Parts 1 & 2
 * File: main.c
 * Description: This program uses a series of functions to initialize and display a
 * blinking cursor on an LCD interfaced to the MSP432 MCU
 **************************************************************************************/
#include "msp.h"
#include "stdio.h"

// Declaring custom functions
void PinInit(void);
void SysTick_Init(void);
void SysTick_delay(uint16_t delay);
void TimerA0Init_Emit(void);
void TimerA2Init_Rec(void);

// Declaring global variables to be used in interrupt
volatile uint16_t currentedge, lastedge, period;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    // Calling initialization functions
    PinInit();
    TimerA0Init_Emit();
    TimerA2Init_Rec();

    while (1)
    {
            P1->OUT &= ~BIT0;   // LED off when interrupt not triggered
    }
}

/****| PinInit | *****************************************
 * Brief: This function is used to initialize the on board
 * red LED.
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void PinInit(void)
{
    // Red LED Initialization
    P1->SEL1 &= ~BIT0;          // Configure P1.0 as simple I/O
    P1->SEL0 &= ~BIT0;
    P1->DIR |= BIT0;            // P1.0 set as output pin
}

/****| TimerA0Init_Emit | *****************************************
 * Brief: This function is used for the initialization
 * of Timer A0.1, which is used by the IR transmitter to generate
 * 10 Hz pulses.
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void TimerA0Init_Emit(void)
{
    P2->SEL0 |= BIT4;
    P2->SEL1 &=~ BIT4;
    P2->DIR |= BIT4;                            // P2.4 set TA0.1

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK |   // SMCLK
            TIMER_A_CTL_MC__UP |                // Up mode
            TIMER_A_CTL_CLR |                   // Clear timer
            TIMER_A_CTL_ID__8;                  // Divide clock by 8

    TIMER_A0->CCR[0] = 37509;                   // 10 Hz
    TIMER_A0->CCR[1] = 37509/2;                 // 50% duty cycle

    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_OUTMOD_7; // CCR1 reset/set mode 7

}

/****| TimerA2Init_Rec | *****************************************
 * Brief: This function is used for the initialization
 * of Timer A2.3, which is used by the IR receiver to capture
 * a period on P6.6
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void TimerA2Init_Rec(void)
{
    // Initializing TA2.3
    P6->SEL0 |= BIT6;       // Input capture pin, second function
    P6->SEL1 &= ~BIT6;      // Input capture pin, second function
    P6->DIR &= ~ BIT6;

    TIMER_A2->CCR[0] = 65535;                       // Max count
    TIMER_A2->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7;     // Reset/set mode 7

    TIMER_A2->CTL |= TIMER_A_CTL_TASSEL_2 |         // Use SMCLK as clock source,
            TIMER_A_CTL_MC__UP |                    // Start timer in UP mode
            TIMER_A_CTL_ID_3 |                      // Divide by 8 375KHz to capture 10 & 14 Hz
            TIMER_A_CTL_CLR;                        // Clear TA2R

    TIMER_A2->CCTL[3] =TIMER_A_CCTLN_CM_1 |         // Capture rising edge,
            TIMER_A_CCTLN_CCIS_0 |                  // Use CCI2A
            TIMER_A_CCTLN_CCIE |                    // Enable capture interrupt
            TIMER_A_CCTLN_CAP |                     // Enable capture mode,
            TIMER_A_CCTLN_SCS;                      // Synchronous capture

    NVIC_EnableIRQ(TA2_N_IRQn);                     // Enable interrupt in NVIC vector
    __enable_irq();                                 // Enable global interrupt
}

/****| TA2_N_IRQHandler | *****************************************
 * Brief: This function is triggered when the IR receiver picks up
 * a signal from the IR transmitter. The period is calculated and
 * if it is within 5% of the 10Hz period, the LED will turn on.
 * param:
 * (void) data: N/A
 * return:
 * N/A
 *************************************************************/
void TA2_N_IRQHandler(void)
{
    TIMER_A2->CCTL[3] &= ~1;                        // Clear the TA.2 interrupt flag
    currentedge = TIMER_A2->CCR[3];                 // Store current edge value in a variable
    period = currentedge - lastedge;                // Store period in a variable
    lastedge = currentedge;                         // Continuously updating last edge


    if ((35635 < period) && (period < 39375))       // If period is within 5% of 10Hz
    {
        P1->OUT |= BIT0;                            // Turn LED turn on
    }

    TIMER_A2->CCTL[3] &= ~(TIMER_A_CCTLN_CCIFG);    // Clear interrupt flag
    printf("test\n");
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


