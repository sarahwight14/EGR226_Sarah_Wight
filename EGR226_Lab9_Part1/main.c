/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/24/2021
* Project: Lab 9 Part 1
* File: main.c
* Description: This program uses GPIO interrupts and Timer A to generate a PWM signal
* that is connected directly to an I/O pin for controlling the speed of a DC motor.
* When the green button is pressed, the speed increases by 10%. When the blue button
* is pressed, the speed decreases by 10%. When the red button is pushed,
* the DC motor stops.
**************************************************************************************/

#include "msp.h"

// Custom Function Declarations
void Pins_Init(void);               // Initialization function for pins and timer A
void SysTick_Init(void);            // Initialization function for systick timer
void SysTick_delay(uint16_t delay); // Systick millisecond delay function
void PORT5_IRQHandler(void);        // ISR handler function
void Debounce_Switch(void);         // Debounce function

// Declaring global variables for ISR
volatile float dutyCycle = 0;
volatile int flag = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    __disable_irq();                // Disable global interrupts

    Pins_Init();                    // Calling pin initialization function
    SysTick_Init();                 // Calling systick initialization function

    NVIC_EnableIRQ(PORT5_IRQn);     // Enable Port 5 interrupts on the NVIC
    __enable_irq();                 // Enable global interrupts

    while (1)
    {
        ;     // Hardware is doing all work - nothing to do in this while loop
    }
}

/****| Pins_Init | *****************************************
* Brief: This function is used for the initialization
* of Timer A and the pushbutton switches
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void Pins_Init(void)
{
    // Configure GPIO for PWM output
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~(BIT4);
    P2->DIR |= BIT4;
    P2->OUT &= ~BIT4;

    TIMER_A0->CCR[0] = 30000;       // PWM period (# cycles of clock)
    TIMER_A0->CCTL[1] = 0xE0;       // CCR1 reset/set mode 7
    TIMER_A0->CCR[1] = 0;           // CCR1 PWM duty cycle
    TIMER_A0->CTL = 0x0214;         // SMCLK, Up mode, clear TAR to start

    // Button Initialization
    P5->SEL1 &= ~0x07;              // Configure Port 5 as simple I/O
    P5->SEL0 &= ~0x07;
    P5->DIR &= ~0x07;               // Port 5 set as input
    P5->REN |= 0x07;                // Port 5 pull resistor enabled
    P5->OUT |= 0x07;
    P5->IES |= 0x07;                // Set pin interrupt to trigger when it goes from high to low
    P5->IE |= 0x07;                 // Enable interrupt for Port 5
    P5->IFG = 0;                    // Clear all Port 5 interrupt flags
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
    SysTick->CTRL = 0;             // Disable SysTick
    SysTick->LOAD = 0x00FFFFFF;    // Max reload value
    SysTick->VAL = 0;              // Any write to current clears it
    SysTick->CTRL = 0x00000005;    // Enable systick, 3MHz, no interrupts
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

/****| Debounce_Switch | *****************************************
* Brief: This function checks to see if a button is pushed to
* deal with switch bounce
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void Debounce_Switch(void)
{
    flag = 0;                           // Set flag to 0

    if ((P5->IN & 0x07) != 0x07)        // If button is pushed
    {
        SysTick_delay(100);             // Pause for switch bounce
        if ((P5->IN & 0x07) != 0x07)    // Check if button is still pushed
            flag = 1;                   // If button is pushed, set flag to 1
    }
}

/****| Port5_IRQHandler | *****************************************
* Brief: This function gives instruction to Port 5 when an
* interrupt is detected. It either increases, decreases, or stops
* the duty cycle of the DC motor, depending on which button
* is pressed
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void PORT5_IRQHandler(void)
{
    // If P5.0, P5.1, or P5.2 had an interrupt, call debounce function
    if (P5->IFG & 0x07)
        Debounce_Switch();

    while (flag == 1)
    {
        // If green button is pressed (P5.0 had an interrupt)
        if ((P5->IN & BIT0) != BIT0)
        {
            // If duty cycle is already at max, do not increase speed
            if (dutyCycle >= 30000)
            {
                dutyCycle = 30000;
            }

            // If duty cycle is below max, increase speed by 10%
            else
            {
                dutyCycle += 3000;
            }

            TIMER_A0->CCR[1] = dutyCycle;   // Setting duty cycle
            SysTick_delay(1);               // Delay
        }

        // If blue button is pressed (P5.1 had an interrupt)
        if ((P5->IN & BIT1) != BIT1)
        {
            // If duty cycle is already at min, do not decrease speed
            if (dutyCycle <= 0)
            {
                dutyCycle = 0;
            }

            // If duty cycle is above min, decrease speed by 10%
            else
            {
                dutyCycle -= 3000;
            }

            TIMER_A0->CCR[1] = dutyCycle;   // Setting duty cycle
            SysTick_delay(1);               // Delay

        }

        // If red button is pressed (P5.2 had an interrupt)
        if ((P5->IN & BIT2) != BIT2)
        {
            dutyCycle = 0;                  // Stop DC motor
            TIMER_A0->CCR[1] = dutyCycle;   // Setting duty cycle
            SysTick_delay(1);               // Delay
        }

        flag = 0;                           // Set flag to 0
    }

    P5->IFG = 0;                            // Clear all flags
}
