/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 2/17/2021
* Project: Lab 4 Part 1
* File: main.c
* Description: This program drives LED's interfaced to port pins on the MSP432 MCU
* based on input from one of the on board pushbuttons. Each time the switch
* is depressed, a different colored LED will light
**************************************************************************************/

#include "msp.h"


void main(void)
{
    int i = 0;                  // Variable declaration for a counter


    // Button 1 Initialization
    P1->SEL1 &= ~BIT1;          // Configure P1.1 as simple I/O
    P1->SEL0 &= ~BIT1;
    P1->DIR &= ~BIT1;           // P1.1 set as input
    P1->REN |= BIT1;            // P1.1 pull resistor enabled
    P1->OUT |= BIT1;


    // Red LED Initialization
    P2->SEL1 &= ~BIT0;          // Configure P2.0 as simple I/O
    P2->SEL0 &= ~BIT0;
    P2->DIR |= BIT0;            // P2.0 set as output pin


    // Green LED Initialization
    P2->SEL1 &= ~BIT1;          // Configure P2.1 as a simple I/O
    P2->SEL0 &= ~BIT1;
    P2->DIR |= BIT1;            // P2.1 set as output pin


    // Blue LED Initialization
    P2->SEL1 &= ~BIT2;          // Configure P2.2 as a simple I/O
    P2->SEL0 &= ~BIT2;
    P2->DIR |= BIT2;            // P2.2 set as output pin


    // Turns off all LEDs upon startup or program reset
    P2->OUT &= ~BIT0;
    P2->OUT &= ~BIT1;
    P2->OUT &= ~BIT2;


    while (1)
    {
        // If button is pressed, delay switch
        if ((P1->IN & BIT1) != BIT1)
        {
            __delay_cycles(30000);

            // If button is not pressed, increment counter
            if ((P1->IN & BIT1) == BIT1)
            {
                i++;
            }
        }

            if (i == 0)
            {
                P2->OUT &= ~BIT2;           // Turns off blue LED
                P2->OUT |= BIT0;            // Turns on red LED
            }

            else if (i == 1)
            {
                P2->OUT &= ~BIT0;           // Turns off red LED
                P2->OUT |= BIT1;            // Turns on green LED
            }

            else if (i == 2)
            {
                P2->OUT &= ~BIT1;           // Turns off green LED
                P2->OUT |= BIT2;            // Turns on blue LED
            }

            else
            {
                i = 0;                      // Sets counter back to 0 to enable looping
            }
    }
}
