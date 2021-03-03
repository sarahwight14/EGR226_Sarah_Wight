/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/3/2021
* Project: Lab 6 Part 2
* File: main.c
* Description: This program uses the MSP432 serial port to prompt the user to enter and
* collect input from the keypad to create a 4 digit PIN code.
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
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    int pin[4];         // array for pin
    int temp[500];      // temporary array that reads in user input
    int i=0;            // counter for temporary array
    int j;              // secondary counter

    // Calling initialization functions
    keypad_init();
    SysTick_Init();

    // Prompting user to enter a 4 digit pin code
    printf("Enter a 4 digit PIN:\n");

    while (1)
    {
        // Calling function to read keypad
        pressed = keypad_read();

        // If a button is pressed, call print function then delay before reading again
        if (pressed)
        {
            SysTick_delay(10);

            // Checking if button is still pressed to deal with switch bounce
            if (pressed)
            {
                print_keys(num);
                SysTick_delay(10);

                // Storing entries in a temporary array and incrementing counter
                // for all numbers except 10 and 12 (* and #)
                if (num!=10 && num!=12)
                {
                    temp[i] = num;
                    i++;
                }
            }
         }

        // 4 digit pin printed once user enters the pound sign
        if (num == 12)
         {

            // Display message if user enters less than 4 inputs
            if (i < 4)
             {
                 printf("Too few inputs\n");
                 num = 0;   // setting num to zero to avoid infinite loop
             }

             else
             {
                 // Setting each spot in the pin array equal to the last 4 entries
                 // in the temporary variable
                 pin[0] = temp[i-4];
                 pin[1] = temp[i-3];
                 pin[2] = temp[i-2];
                 pin[3] = temp[i-1];

                 // Cycling through the entered numbers & changing any 11's to 0's
                 for (j=0; j<4; j++)
                 {
                     if (pin[j] == 11)
                         pin[j] = 0;
                 }

                 // Printing last four entries to the screen
                 printf("Pin Entered: [%d] [%d] [%d] [%d]\n", pin[0], pin[1], pin[2], pin[3]);

                 i = 0;     // setting counter back to 0
                 num = 0;   // setting num to 0 to loop again
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
    // Initialization of pins
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


/****| keypad_read | *****************************************
* Brief: This function reads the input from the keypad
* param:
* (void) data: N/A
* return:
* (uint8_t) returns an integer to allow program to proceed
*************************************************************/
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

