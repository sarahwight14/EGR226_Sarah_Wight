/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/17/2021
* Project: Lab 7 Part 1
* File: main.c
* Description: This program uses a series of functions to initialize and display a
* blinking cursor on an LCD interfaced to the MSP432 MCU
**************************************************************************************/

#include "msp.h"

void SysTick_Init(void);
void LCD_init(void);
void SysTick_delay(uint16_t delay);
void commandWrite(uint8_t command);
void pushNibble(uint8_t data, uint8_t control);


int main(void)
{
    SysTick_Init();
    LCD_init();

    while (1)
    {
        commandWrite(1);        // Clear display
        SysTick_delay(500);
        commandWrite(0x80);     // Set cursor to first spot of line 1
        SysTick_delay(500);     // Blinking cursor
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

/****| LCD_init | *****************************************
* Brief: This function is used for the initialization
* of the LCD
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void LCD_init(void)
{
    // Setting pins 4.0, 4.2, 4.4-4.7 to output
    P4->DIR |= 0xF5;

    // Reset Sequence
    commandWrite(3);
    SysTick_delay(100);
    commandWrite(3);
    SysTick_delay(1);
    commandWrite(3);
    SysTick_delay(100);

    // Setting 4-bit Mode
    commandWrite(2);

    // 2 Lines, 5x7 Format
    SysTick_delay(100);
    commandWrite(0x28);
    SysTick_delay(1);

    // Display ON, Cursor ON, Blinking
    SysTick_delay(1);
    commandWrite(0x0F);

    // Clear Display, Move Cursor to Home Position
    SysTick_delay(1);
    commandWrite(0x01);

    // Increment Cursor
    SysTick_delay(1);
    commandWrite(0x06);
    SysTick_delay(10);
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

/****| commandWrite | *****************************************
* Brief: This function writes one byte of command by calling
* the pushNibble() function twice with the command parameter
* param:
* (uint8_t) data: one integer worth of data that gives a
* command to the LCD
* return:
* N/A
*************************************************************/
void commandWrite(uint8_t command)
{
    pushNibble(command & 0xF0, 0);  // Sending upper nibble
    pushNibble(command << 4, 0);    // Sending lower nibble

    // commands 1 and 2 require more delay
    if (command < 4)
        SysTick_delay(4);
    else
        SysTick_delay(1);
}

/****| pushNibble | *****************************************
* Brief: This function pushes 1 nibble onto the data pins and
* pulses the enable pin
* param:
* (uint8_t) data: two integers worth of data used to assign bits
* to data pins
* return:
* N/A
*************************************************************/
void pushNibble(uint8_t data, uint8_t control)
{
    P4->OUT &= ~0x10;               // Set RS = 0

    data &= 0xF0;                   // Assign most significant 4 bits to data pins
    control &= 0x0F;                // Assign least significant 4 bits to data pins

    P4->OUT = data | control | 4;   // Pulse E
    SysTick_delay(1);
    P4->OUT = data;                 // Clear E
    P4->OUT = 0;
}
