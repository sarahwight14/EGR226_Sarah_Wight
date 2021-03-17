/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/17/2021
* Project: Lab 8 Part 3
* File: main.c
* Description: This program uses Timer A to generate a PWM signal by
* reading the duty cycle from a keypad.
**************************************************************************************/

#include "msp.h"
#include <stdio.h>

// Declaring custom functions for later use
void timerA_Init(void);
void timerA(uint16_t duty);

void SysTick_Init(void);
void SysTick_delay(uint16_t delay);

void keypad_init(void);
void user_cycle(void);
uint8_t keypad_read(void);
void print_keys(void);


volatile float dutyCycle = 20;      // Global variable to be adjusted while code is running
volatile float period = 37500;      // Period to attain 40 Hz frequency

// Declaring global variables
uint8_t flag = 1;
uint8_t num, pressed, code;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // Stop watchdog timer

    float sendDutyCycle = 0;     // Declaring variable to be sent to Timer A function

    SysTick_Init();              // Calling initialization function for systick timer
    timerA_Init();               // Calling initialization function for timer A
    keypad_init();               // Calling initialization function for keypad

    // Prompting user to enter duty cycle
    printf("Enter Duty Cycle:\n");

    while(1)
    {
        code = 0;                // Clearing the duty cycle


        while(flag == 1)
        {
            user_cycle();        // Function for user to enter duty cycle
        }

        sendDutyCycle = ((((float)code * 10) / 100) * period);     // Calculation for time on

        timerA((int)sendDutyCycle);     // Typecasting time on to int to fit parameters of Timer A function
        SysTick_delay(50);              // Delay
        flag = 1;                       // Enable flag to read in duty cycle

        printf("Enter Duty Cycle\n");   // Prompting user to enter duty cycle again
    }
}


void keypad_init(void)
{
    // Initialization of pins
    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;
    P4->REN |= 0x0F;
    P4->OUT |= 0x0F;
}

void timerA_Init(void)
{
    // Configure GPIO for PWM output
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~(BIT4);
    P2->DIR |= BIT4;
}


void timerA(uint16_t dc)
{
    TIMER_A0->CCR[0] = period - 1;                  // PWM period (# cycles of clock)
    TIMER_A0->CCTL[1] = 0xE0;                       // CCR1 reset/set mode 7
    TIMER_A0->CCR[1] = dc;                          // CCR1 PWM duty cycle
    TIMER_A0->CTL = 0x0214;                         // SMCLK, Up mode, clear TAR to start
}


void SysTick_Init(void)
{
    SysTick->CTRL = 0;            // Disable SysTick
    SysTick->LOAD = 0x00FFFFFF;   // Max reload value
    SysTick->VAL = 0;             // Any write to current clears it
    SysTick->CTRL = 0x00000005;   // Enable systick, 3MHz, no interrupts
}


void SysTick_delay(uint16_t delay)
{
    SysTick->LOAD = ((delay * 3000) - 1);       // Delay 1 msecond per delay value
    SysTick->VAL = 0;                           // Any write to CVR clears it
    while ((SysTick->CTRL & 0x00010000) == 0);  // Wait for flag to be SET
}


void user_cycle(void)
{
    // Calling function to read keypad
    pressed = keypad_read();
    if(pressed)
    {
        print_keys();   // Printing entry

        // Re prompt user if asterisk is entered
        if (num == 10)
        {
            printf("Please enter a # between 1-9");
        }

        // If pound sign is entered, print duty cycle
        else if (num == 12)
        {
            printf("Duty Cycle: \n");
            printf("%d%% \n\n", code*10);

            // Making sure entry does not exceed maximum possible duty cycle
            if (code > 90)
            {
                printf("Duty Cycle must be between 0 and 100%\n");
            }

            // If within possible duty cycle, set flag to 0 and exit
            else
            {
                flag = 0;
            }
        }

        // Setting duty cycle equal to user entry
        else
        {
            code = num;
        }
    }
    SysTick_delay(10);      // Delay before executing loop again
}


uint8_t keypad_read(void) // function for reading in keypad presses
{
    uint8_t col, row;

    for (col = 0; col < 3; col++)
    {
        P4->DIR = 0x00;             // Setting columns to inputs
        P4->DIR |= BIT(4+col);      // Setting column 3 to output
        P4->OUT &=~ BIT(4+col);     // Setting column 3 to LOW

        SysTick_delay(10);          // Delay the while loop
        row = P4->IN & 0x0F;        // Read all rows

        while (!(P4IN & BIT0) | !(P4IN & BIT1) | !( P4IN & BIT2) | !( P4IN & BIT3));

        if (row != 0x0F)     // If an input is low, some key is pressed.
            break;
    }

    P4->DIR = 0x00;         // Setting columns to inputs
    if (col == 3)
        return 0;

    if (row == 0x0E) num = col + 1;          // Key in row 0
    if (row == 0x0D) num = 3 + col + 1;      // Key in row 1
    if (row == 0x0B) num = 6 + col + 1;      // Key in row 2
    if (row == 0x07) num = 9 + col+ 1;       // Key in row 3

    return 1;
}


void print_keys(void) // function to print key presses
{
    // Print if 1-9 is entered
    if (num < 10)
    {
        printf("%d\n", num);
    }

    else if (num >= 10)
    {
        // Print asterisk if 10 is entered
        if (num == 10)
        {
            printf("*\n");
        }

        // Print 0 if 11 is entered
        else if (num == 11)
        {
            num = 0;
            printf("0\n");
        }

        // Print pound sign if 12 is entered
        else if (num == 12)
        {
            printf("#\n");
        }
    }
}


