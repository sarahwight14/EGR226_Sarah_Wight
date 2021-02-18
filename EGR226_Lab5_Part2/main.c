/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 2/17/2021
* Project: Lab 5 Part 2
* File: main.c
* Description: This program controls the sequence of flashing LED's using time delay.
* The program sequences through the colors as long as the pushbutton is pressed.
**************************************************************************************/

#include "msp.h"


void main(void)
{
    int i = 0;                  // Variable declaration for a counter

    // Button 1 Initialization
        P5->SEL1 &= ~BIT6;          // Configure P1.1 as simple I/O
        P5->SEL0 &= ~BIT6;
        P5->DIR &= ~BIT6;           // P1.1 set as input
        P5->REN |= BIT6;            // P1.1 pull resistor enabled
        P5->OUT |= BIT6;


        // Red LED Initialization
        P1->SEL1 &= ~BIT6;          // Configure P2.0 as simple I/O
        P1->SEL0 &= ~BIT6;
        P1->DIR |= BIT6;            // P2.0 set as output pin


        // Yellow LED Initialization
        P5->SEL1 &= ~BIT0;          // Configure P2.1 as a simple I/O
        P5->SEL0 &= ~BIT0;
        P5->DIR |= BIT0;            // P2.1 set as output pin


        // Green LED Initialization
        P3->SEL1 &= ~BIT6;          // Configure P2.2 as a simple I/O
        P3->SEL0 &= ~BIT6;
        P3->DIR |= BIT6;            // P2.2 set as output pin


        // Turns off all LEDs upon startup or program reset
        P1->OUT &= ~BIT6;
        P5->OUT &= ~BIT0;
        P3->OUT &= ~BIT6;


    while (1)
    {
        // While button is pressed loop executes
        while ((P5->IN & BIT6) != BIT6)
        {

            // Sets counter back to 0 and loops through colors again
            if(i == 3)
            {
                i=0;
            }

            if (i == 0)
            {
                P3->OUT &= ~BIT6;           // Turns off green LED
                P1->OUT |= BIT6;            // Turns on red LED
                __delay_cycles(3000000);    // Delay for 1 second
                i++;                        // Increment counter
            }

            else if (i == 1)
            {
                P1->OUT &= ~BIT6;           // Turns off red LED
                P5->OUT |= BIT0;            // Turns on yellow LED
                __delay_cycles(3000000);    // Delay for 1 second
                i++;                        // Increment counter
            }

            else if (i == 2)
            {
                P5->OUT &= ~BIT0;           // Turns off yellow LED
                P3->OUT |= BIT6;            // Turns on green LED
                __delay_cycles(3000000);    // Delay for 1 second
                i++;                        // Increment counter
            }
        }
    }
}
