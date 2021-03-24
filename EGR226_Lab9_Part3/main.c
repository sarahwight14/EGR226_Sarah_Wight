/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 3/24/2021
* Project: Lab 9 Part 3
* File: main.c
* Description: This program uses GPIO interrupts to control what is displayed on a
* 7 segment LED. When the green button is pressed, the number displayed increments
* by one. When the blue button is pressed, the number displayed decrements by one.
**************************************************************************************/

#include "msp.h"

// Custom Function Declarations
void Pins_Init(void);                   // Initialization function for pins and timer A
void SysTick_Init(void);                // Initialization function for systick timer
void SysTick_delay(uint16_t delay);     // Systick millisecond delay function
void LED_Counter(void);                 // Function to change number on LED
void PORT5_IRQHandler(void);            // ISR handler function
void Debounce_Switch(void);             // Debounce function


// Declaring global variables for ISR
volatile int count = 0;
volatile int flag = 0;

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    __disable_irq();                // Disable global interrupts

    Pins_Init();                    // Calling pin initialization function
    SysTick_Init();                 // Calling systick initialization function

    NVIC_EnableIRQ(PORT5_IRQn);     // Enable Port 5 interrupts on the NVIC
    __enable_irq();                 // Enable global interrupts

    P4->OUT |= 0x3F;                // Display 0 on LED

    while (1)
    {
        ;       // Hardware is doing all work - nothing to do in this while loop
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

    // Button Initialization
    P5->SEL1 &= ~0x03;          // Configure Port 5 as simple I/O
    P5->SEL0 &= ~0x03;
    P5->DIR &= ~0x03;           // Port 5 set as input
    P5->REN |= 0x03;            // Port 5 pull resistor enabled
    P5->OUT |= 0x03;
    P5->IES |= 0x03;            // Set pin interrupt to trigger when it goes from high to low
    P5->IE |= 0x03;             // Enable interrupt for Port 5
    P5->IFG = 0;                // Clear all Port 5 interrupt flags
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
    if ((P5->IN & 0x03) != 0x03)        // If button is pushed
    {
        SysTick_delay(10);              // Pause for switch bounce
        if ((P5->IN & 0x03) != 0x03)    // Check if button is still pushed
            flag = 1;                   // If button is pushed, set flag to 1
    }
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
    // Determines number displayed
    switch(count)
    {
    case 0:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x3F;        // Display 0
        break;
    case 1:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x06;        // Display 1
        break;
    case 2:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x5B;        // Display 2
        break;
    case 3:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x4F;        // Display 3
        break;
    case 4:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x66;        // Display 4
        break;
    case 5:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x6D;        // Display 5
        break;
    case 6:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x7D;        // Display 6
        break;
    case 7:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x07;        // Display 7
        break;
    case 8:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x7F;        // Display 8
        break;
    case 9:
        P4->OUT &= ~0x7F;       // Clear
        P4->OUT |= 0x6F;        // Display 9
        break;
    default:
        P4->OUT |= 0x3F;        // Display 0 as default
        break;
    }
}

/****| Port5_IRQHandler | *****************************************
* Brief: This function gives instruction to Port 5 when an
* interrupt is detected. It either increments or decrements
* the count variable, depending on which button is pressed
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void PORT5_IRQHandler(void)
{
    // If P5.0 or P5.1 had an interrupt, call debounce function
    if (P5->IFG & 0x03)
    {
        Debounce_Switch();

        while (flag == 1)
        {
            // If green button is pressed (P5.0 had an interrupt), increment counter
            if ((P5->IN & BIT0) != BIT0)
            {
                count++;

                // If count is greater than 9, reset count to 0
                if (count > 9)
                {
                    count = 0;
                }
                LED_Counter();      // Function call to display count on LED
            }

            // If blue button is pressed (P5.1 had an interrupt), decrement counter
            if ((P5->IN & BIT1) != BIT1)
            {
                count--;

                // If count is less than 0, reset count to 9
                if (count < 0)
                {
                    count = 9;
                }
                LED_Counter();      // Function call to display count on LED
            }

            flag = 0;               // Set flag to 0

            P5->IFG &= ~0x03;       // Clear all flags
        }
    }
}
