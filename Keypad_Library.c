/**************************************************************************************
*                                Keypad_Library.c
*                                  Sarah Wight
*                         EGR226      Date: April 10, 2021
*
*  This is a library for the keypad.
*
*  All functions are briefly described in their comment blocks.  The /// notation makes
*  it so the function description block is visible when you hovering over a function call
*  in any file (this feature is called Intellisense).
*
* **************************************************************************************/

#include <Keypad_Library.h>
#include "msp.h"
#include <SysTick_Library.h>

/// /****| keypad_init | *****************************************
/// * Brief: This function initializes the pins that are connected
/// * to the keypad
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void keypad_init(void)
{
    // initilization of pins
    P4->SEL0 &= ~0x7F;
    P4->SEL1 &= ~0x7F;
    P4->DIR &= ~0x7F;
    P4->REN |= 0x0F;
    P4->OUT |= 0x0F;
}


/// /****| keypad_read | ********************************************
/// * Brief: This function reads the input from the keypad
/// * param:
/// * (void) data: N/A
/// * return:
/// * (uint8_t) returns a single integer to allow program to proceed
/// ****************************************************************/
uint8_t keypad_read(void)
{
    uint8_t col, row;

    for (col = 0; col < 3; col++)
    {
        P4->DIR = 0x00;             // Setting columns to inputs
        P4->DIR |= BIT(4+col);      // Setting column 3 to output
        P4->OUT &= ~BIT(4+col);     // Setting column 3 to LOW

        SysTick_delay_ms(10);       // Delay the while loop
        row = P4->IN & 0x0F;        // Read all rows

        while (!(P4IN & BIT0) | !(P4IN & BIT1) | !(P4IN & BIT2) | !(P4IN & BIT3));

        if (row != 0x0F)            // If an input is low, a key is pressed
            break;
    }

    P4->DIR = 0x00;                 // Setting columns to inputs
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


/// /****| print_keys | *****************************************
/// * Brief: This function prints the numbers/symbols that are
/// * entered on the keypad.
/// * param:
/// * (uint16_t) data: one integer worth of data input that
/// * determines the duration of the delay
/// * return:
/// * N/A
/// *************************************************************/
void print_keys(void)
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
        num = 0;
        printf("Key: [0]\n");
    }

    // Print a pound sign if 12 is entered
    else if (num == 12)
    {
        printf("Key: [#]\n");
    }
}
