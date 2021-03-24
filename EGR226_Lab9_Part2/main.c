/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/24/2021
* Project: Lab 9 Part 2
* File: main.c
* Description: This program uses Systick interrupts to allow a 7 segment LED to display
* the numbers 0-9 in one second intervals, cycling through this sequence repeatedly
**************************************************************************************/

#include "msp.h"

// Custom Function Declarations
void Pins_Init(void);                   // Initialization function for pins and timer A
void SysTick_Init(void);                // Initialization function for systick timer
void SysTick_delay(uint16_t delay);     // Systick millisecond delay function
void SysTick_Handler(void);             // Systick handler function
void LED_Counter(void);                 // Function to change number on LED
void Debounce_Switch(void);             // Debounce function

// Declaring global variables for ISR
volatile int count = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    __disable_irq();                // Disable global interrupts

    Pins_Init();                    // Calling pin initialization function
    SysTick_Init();                 // Calling systick initialization function

    __enable_irq();                 // Enable global interrupts

    while (1)
    {
        LED_Counter();              // Calling function that displays #'s on LED

        if(count>9)                 // Once the count gets to 9, start back at 0
            count = 0;
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
    // 7 Segment LED initialization
    P4->SEL1 &= ~0x7F;          // Configure Port 4 as simple I/O
    P4->SEL0 &= ~0x7F;
    P4->DIR |= 0x7F;            // Port 4 set as output
    P4->OUT &= ~0x7F;           // Port 4 set to LOW
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
    SysTick->CTRL = 0x00000007;   // Enable systick, 3MHz, with interrupts
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

/****| LED_Counter | *****************************************
* Brief: This function displays numbers on the 7 segment LED
* by using a switch statement to determine which number to
* display based on the counter status
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void LED_Counter(void)
{
    // Determines number displayed based on counter status
    switch(count)
    {
    case 0:
            P4->OUT |= 0x3F;        // Display 0
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 1:
            P4->OUT |= 0x06;        // Display 1
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 2:
            P4->OUT |= 0x5B;        // Display 2
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 3:
            P4->OUT |= 0x4F;        // Display 3
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 4:
            P4->OUT |= 0x66;        // Display 4
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 5:
            P4->OUT |= 0x6D;        // Display 5
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 6:
            P4->OUT |= 0x7D;        // Display 6
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 7:
            P4->OUT |= 0x07;        // Display 7
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 8:
            P4->OUT |= 0x7F;        // Display 8
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    case 9:
            P4->OUT |= 0x6F;        // Display 9
            SysTick_delay(1000);    // Delay 1 second
            P4->OUT &= ~0x7F;       // Clear
            break;
    default:
        P4->OUT |= 0x3F;            // Display 0 as default
        break;
    }
}

/****| SysTick_Handler | *****************************************
* Brief: This function increments the counter variable when
* interrupt is detected
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void SysTick_Handler(void)
{
    count++;    // Increment counter
}
