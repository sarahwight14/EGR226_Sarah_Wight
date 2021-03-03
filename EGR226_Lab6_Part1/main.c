/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/3/2021
* Project: Lab 6 Part 1
* File: main.c
* Description: This program uses the MSP432 serial port to send the key press
* information collected to the CCS console window for display.
**************************************************************************************/

#include "msp.h"
#include <stdio.h>

// Declaring functions for later use
void keypad_init(void);
void SysTick_Init(void);
uint8_t keypad_read(void);
void print_keys(uint8_t num);
void SysTick_delay(uint16_t delay);

// Declaring global variables
uint8_t num, pressed;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Calling initialization functions
	keypad_init();
	SysTick_Init();

	while (1)
	{
	    // Calling function to read keypad
	    pressed = keypad_read();
	    if (pressed)
	    {
	        SysTick_delay(10);

	        // Checking if button is still pressed to deal with switch bounce
	        if (pressed)
	        {
	            print_keys(num);
	            SysTick_delay(10);
	        }
	    }
	}

}


/****| keypad_init | *****************************************
* Brief: This function initializes the pins that are connected
* to the keypad
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void keypad_init(void)
{
    // initilization of pins
    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;
    P4->REN |= 0x0F;
    P4->OUT |= 0x0F;
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
    SysTick -> CTRL = 0;              // disable SysTick during step
    SysTick -> LOAD = 0x00FFFFFF;     // max reload value
    SysTick -> VAL = 0;               // any write to current clears it
    SysTick -> CTRL = 0x00000005;     // enable SysTick, 3MHz, no interrupts
}


/****| keypad_read | ********************************************
* Brief: This function reads the input from the keypad
* param:
* (void) data: N/A
* return:
* (uint8_t) returns a single integer to allow program to proceed
****************************************************************/
uint8_t keypad_read(void)
{
    uint8_t col, row;

    for (col = 0; col < 3; col++)
    {
        P4->DIR = 0x00;             // Setting columns to inputs
        P4->DIR |= BIT(4+col);      // Setting column 3 to output
        P4->OUT &= ~BIT(4+col);     // Setting column 3 to LOW

        SysTick_delay(10);          // Delay the while loop
        row = P4->IN & 0x0F;        // Read all rows

        while (!(P4IN & BIT0) | !(P4IN & BIT1) | !(P4IN & BIT2) | !(P4IN & BIT3));

        if (row != 0x0F)            // If an input is low, a key is pressed
            break;
    }

    P2->DIR = 0x00;                 // Setting columns to inputs
    if (col==3)
        return 0;

    if (row == 0x0E)                // Key in row 0
        num = col+1;
    if (row == 0x0D)                // Key in row 1
        num = 3 + col + 1;
    if (row == 0x0B)                // Key in row 2
        num = 6 + col + 1;
    if (row == 0x07)                // Key in row 3
        num = 9 + col + 1;

    return 1;
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
    SysTick -> LOAD = ((delay*3000)-1);           // Delay for 1 millisecond per delay value
    SysTick -> VAL = 0;                           // Any write to CVR clears it
    while ((SysTick -> CTRL & 0x00010000) == 0);  // wait for flag to be SET
}


/****| print_keys | *****************************************
* Brief: This function prints the numbers/symbols that are
* entered on the keypad.
* param:
* (uint16_t) data: one integer worth of data input that
* determines the duration of the delay
* return:
* N/A
*************************************************************/
void print_keys(uint8_t num)
{
    // Print number as long as it is between 1 and 9
    if (num > 0 && num < 10)
    {
            printf("Key: [%d]\n", num);
    }

    // Print asterisk if 10 is entered
    else if (num == 10)
    {
        printf("Key: [*]\n");
    }

    // Print a 0 if 11 is entered
    else if (num == 11)
    {
        printf("Key: [0]\n");
    }

    // Print a pound sign if 12 is entered
    else if (num == 12)
    {
        printf("Key: [#]\n");
    }
}
