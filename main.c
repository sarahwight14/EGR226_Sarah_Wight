/**************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 4/25/2021
* Project: Embedded Systems Design Project (Final Project)
* File: main.c
* Description: This program allows a control system to run using a numeric Keypad and
* 16x4 LCD. The heart of this system is the TI MSP432 Launchpad microcontroller board.
* The microcontroller controls the LCD display menu functions using keypad entry.
*
* - A Main Menu is displayed on the LCD which allows the user to select between three
* options: (1) Door (2) Motor (3) Lights.
* - Selecting (1) will take the user to the Door Menu, where the user may choose to
* (1) Open or (2) Close the door.
* - Selecting (2) will take the user to the Motor Menu, where the user will be asked to
* enter a speed (0-9) for the Motor.
* - Selecting (3) will take the user to the Lights Menu, where the user can select
* between Red, Green or Blue then set the brightness (0-100) of that color. Once set,
* that brightness remains until this menu is navigated to again and that color’s
* brightness is altered.
* - The system displays the Main Menu after each task.
**************************************************************************************/

#include "msp.h"
#include <stdio.h>
#include <string.h>
#include "Keypad_Library.h"
#include "LCD_Library.h"
#include "SysTick_Library.h"
#include "Menus_Library.h"
#include "Init_Library.h"


void State_Machine(void);       // Continuously looping through states
void LCD_Backlight(void);       // Continuously updating backlight brightness

void Arm_Password(void);        // Reads the user entered password for door
void Red_LED(void);             // Changes brightness of the red LED
void Green_LED(void);           // Changes brightness of the green LED
void Blue_LED(void);            // Changes brightness of the blue LED

void PORT5_IRQHandler(void);    // Interrupt function for "emergency stop" button
void Debounce_Switch(void);     // Debounce function for push buttons


// Global variables
uint8_t state;
float code;
int flag = 0;
int pw_flag = 0;
int red_bright = 0, red_flag = 0, green_bright = 0, green_flag = 0, blue_bright = 0, blue_flag = 0;
volatile int stop_flag = 0;


/****| main | ****************************************************
* Brief: This function calls initialization functions for all
* components, enables interrupts, and continuously loops through
* the state machine and checks if the LCD backlight has been
* adjusted.
* param:
* (void) data: N/A
* return:
* N/A
*****************************************************************/
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    __disable_irq();                // Disable global interrupts

    // Initialization
    SysTickInit_NoInterrupts();     // SysTick Timer
    lcdInit();                      // LCD
    keypad_init();                  // Keypad
    DC_Init();                      // DC + push button
    Servo_Init();                   // Servo + on board LEDs
    RGB_Init();                     // RGB LED + push button
    Backlight_Init();               // Timer A and ADC for backlight

    NVIC_EnableIRQ(PORT5_IRQn);     // Enable Port 5 interrupts on the NVIC
    __enable_irq();                 // Enable global interrupts

    lcdClear();                     // Clear display
    lcdWriteCmd(0x0C);              // Turn off cursor

    state = 0;                      // Set state to main menu

    while(1)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        State_Machine();            // Continuously check state
    }
}

/****| State_Machine | *****************************************
* Brief: This function is a state machine that determines the
* state of the program and executes certain commands based on
* user input via keypad.
* param:
* (void) data: N/A
* return:
* N/A
*****************************************************************/
void State_Machine(void)
{
    switch (state)
    {
    // CASE 0: DISPLAY MAIN MENU UNTIL KEYPAD IS PRESSED *********************************************************************/
    case 0:
        Main_Menu();                    // Main Menu displayed on LCD
        flag = 1;                       // Flag set to 1 to enter a while loop

        while (flag)                    // LCD displays Main Menu until flag is reset to 0
        {
            LCD_Backlight();            // Continuously checking for LCD backlight adjustments

            pressed = keypad_read();    // Continues to read keypad until a press is detected
            if (pressed)                // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 1)           // If user enters a 1
                {
                    state = 1;          // Sends user to Door Menu
                    pressed = 0;
                    flag = 0;           // Resets flag to break out of loop
                }

                if (num == 2)           // If user enters a 2
                {
                    state = 3;          // Sends user to Motor Menu
                    pressed = 0;
                    flag = 0;           // Resets flag to break out of loop
                }

                if (num == 3)           // If user enters a 3
                {
                    state = 4;          // Sends user to Lights Menu
                    pressed = 0;
                    flag = 0;           // Resets flag to break out of loop
                }
            }
        }
        break;

    // CASE 1: DISPLAYS DOOR MENU *******************************************************************************************/
    case 1:
        Door_Menu();                    // Door Menu displayed on LCD
        flag = 1;                       // Flag set to 1 to enter a while loop

        while (flag)                    // LCD displays Door Menu until flag is reset to 0
        {
            LCD_Backlight();            // Continuously checking for LCD backlight adjustments
            pressed = keypad_read();    // Continues to read keypad until a press is detected
            if (pressed)
            {
                SysTick_delay_ms(10);   // Delay


                if (pressed)            // Checking if button is still pressed to deal with switch bounce
                {
                    state = 2;          // Sending to case 2 for open/close door state
                    pressed = 0;
                    flag = 0;           // Reset flag to break out of the loop
                }
            }
        }
        break;

    // CASE 2: OPENS OR CLOSES DOOR/TURNS ON GREEN OR RED ON BOARD LED **********************************************************/
    case 2:
        if (num == 1)                   // If the user entered a 1
        {
            Arm_Menu();                 // Arm Menu displayed on LCD
            Arm_Password();             // Sets pw_flag to 1 if incorrect password is entered by user

            if (pw_flag == 1)           // If wrong password is entered
            {
                lcdClear();
                TIMER_A0->CCR[3] = 15000;   // CCR2 PWM duty cycle to 50% - turns on piezzo sounder
            }
            TIMER_A0->CCR[2] = 4500;    // Set duty cycle so door rotates 90 degrees
            P2->OUT &= ~BIT0;           // Turn off red LED
            P2->OUT |= BIT1;            // Turn on green LED
            flag = 0;
        }

        if (num == 2)                   // If the user entered a 2
        {
            Arm_Menu();                 // Arm Menu displayed on LCD
            Arm_Password();             // Sets pw_flag to 1 if incorrect password is entered by user

            if (pw_flag == 1)           // If wrong password is entered
            {
                lcdClear();
                TIMER_A0->CCR[3] = 15000;   // CCR2 PWM duty cycle to 50% - turns on piezzo sounder
            }
            TIMER_A0->CCR[2] = 9000;    // Set duty cycle so door rotates 90 degrees
            P2->OUT &= ~BIT1;           // Turn off green LED
            P2->OUT |= BIT0;            // Turn on red LED
            flag = 0;
        }

        if (num == 3)                   // If the user entered a 3
        {
            Arm_Menu();                 // Arm Menu displayed to LCD
            Arm_Password();             // Read in user password entry & set pw_flag

            if (pw_flag == 0)           // If pw_flag is 0, correct password was entered
            {
                lcdClear();
                TIMER_A0->CCR[3] = 0;   // CCR2 PWM duty cycle to 0% - turns off piezzo sounder
            }
        }
        state = 0;                      // Send back to Main Menu
        break;

    // CASE 3: DISPLAYS MOTOR MENU ****************************************************************************************************/
    case 3:
        Motor_Menu();                   // Motor Menu displayed on LCD
        flag = 1;                       // Flag set to 1 to enter a while loop
        int dutyCycle;                  // Duty cycle variable
        int period;

        period = 30000;                 // Period to attain 40 Hz frequency

        while(flag == 1)                // Stuck in loop until user enters a value
        {
            LCD_Backlight();            // Continuously checking for LCD backlight adjustments
            pressed = keypad_read();    // Continues to read keypad until a press is detected
            if(pressed)
            {
                if (num > 9)            // If duty cycle is greater than 9
                {
                    code = 0;           // Set duty cycle to 0
                    flag = 0;
                }

                else                    // Setting duty cycle equal to user entry
                {
                    code = num;
                    flag = 0;
                }
            }
            SysTick_delay_ms(10);       // Delay before executing loop again
        }
        dutyCycle = ((code / 10) * period);  // Calculation for time on
        TIMER_A0->CCR[1] = dutyCycle;   // Setting CCR1 PWM duty cycle
        SysTick_delay_ms(50);           // Delay
        state = 0;                      // Send back to Main Menu
        break;

    // CASE 4: DISPLAYS LIGHT MENU ******************************************************************************************************/
    case 4:
        Lights_Menu();                  // Lights Menu displayed on LCD
        flag = 1;                       // Flag set to 1 to enter a while loop

        while (flag)                    // LCD displays Lights Menu until flag is reset to 0
        {
            LCD_Backlight();            // Continuously checking for LCD backlight adjustments
            pressed = keypad_read();    // Continues to read keypad until a press is detected
            if (pressed)
            {
                SysTick_delay_ms(10);   // Delay

                if (pressed)            // Checking if button is still pressed to deal with switch bounce
                {
                    state = 5;          // Send to case 6 to turn on RGB LED
                    flag = 0;           // Reset flag to break out of the loop
                }
            }
        }
        break;

        // CASE 5: TURNS ON RGB LED *****************************************************************************************************/
    case 5:
        if (num == 1)       // If user enters a 1
        {
            Red_Menu();     // Red LED Brightness Menu displayed to LCD
            Red_LED();      // Changes brightness of red LED
            state = 0;      // Send back to Main Menu
        }

        if (num == 2)       // If user enters a 2
        {
            Green_Menu();   // Green LED Brightness Menu displayed to LCD
            Green_LED();    // Changes brightness of green LED
            state = 0;      // Send back to Main Menu
        }

        if (num == 3)       // If user enters a 3
        {
            Blue_Menu();    // Blue LED Brightness Menu displayed to LCD
            Blue_LED();     // Changes brightness of blue LCD
            state = 0;      // Send back to Main Menu
        }
        break;
    }
}

/****| LCD_Backlight | *********************************************
* Brief: This function reads in ADC values and converts to a duty
* cycle that sets the backlight of the LCD
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void LCD_Backlight(void)
{
    float result = 0.0;

    ADC14->CTL0 |=1;                      // Starts a conversion
    while(!ADC14->IFGR0);                 // Waits until the conversion is complete
    result = ADC14->MEM[5];               // Immediately stores the value in a variable
    TIMER_A1->CCR[4] = result * 0.0609;   // Setting duty cycle: period / 16400 (highest adc value)
}

/****| Arm_Password | *********************************************
* Brief: This function reads in a 3 digit entry from a user and
* determines if the entry matches a predetermined "passcode".
* A flag is set if the user entry is not correct.
*        *PREDETERMINED PASSCODE IS 258*
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void Arm_Password(void)
{
    int num1 = 0, num2 = 0, num3 = 0, password = 0;

    // TAKING IN FIRST DIGIT
    flag = 1;
    int flag2 = 1;

    while (flag2)                       // Stays in loop until all 3 digits are entered
    {
        LCD_Backlight();                // Continuously checking for LCD backlight adjustments
        while (flag)                    // Stays in loop until digit is entered
        {
            LCD_Backlight();            // Continuously checking for LCD backlight adjustments
            pressed = keypad_read();    // Continues to read keypad until a press is detected
            if (pressed)
            {
                SysTick_delay_ms(10);   // Delay

                if (pressed)            // Checking if button is still pressed to deal with switch bounce
                {
                    if (num == 10 | num == 11 | num == 12)
                    {
                        num = 0;        // Set num to zero if *, 0, or # are pressed
                    }
                    num1 = num*100;
                    flag = 0;           // Reset flag to break out of the loop
                }
            }
        }

        // TAKING IN SECOND DIGIT
        flag = 1;

        while (flag)                    // Stays in loop until digit is entered
        {
            LCD_Backlight();            // Continuously checking for LCD backlight adjustments
            pressed = keypad_read();    // Continues to read keypad until a press is detected
            if (pressed)
            {
                SysTick_delay_ms(10);   // Delay

                if (pressed)            // Checking if button is still pressed to deal with switch bounce
                {
                    if (num == 10 | num == 11 | num == 12)
                    {
                        num = 0;        // Set num to zero if *, 0, or # are pressed
                    }
                    num2 = num*10;
                    flag = 0;           // Reset flag to break out of the loop
                }
            }
        }

        // TAKING IN THIRD DIGIT
        flag = 1;

        while (flag)                    // Stays in loop until digit is entered
        {
            LCD_Backlight();            // Continuously checking for LCD backlight adjustments
            pressed = keypad_read();    // Continues to read keypad until a press is detected
            if (pressed)
            {
                SysTick_delay_ms(10);   // Delay

                if (pressed)            // Checking if button is still pressed to deal with switch bounce
                {
                    if (num == 10 | num == 11 | num == 12)
                    {
                        num = 0;        // Set num to zero if *, 0, or # are pressed
                    }
                    num3 = num;
                    flag = 0;           // Reset flag to break out of the loop
                }
            }
        }
        flag2=0;                        // Once all three digits have been entered flag is set to 0 to break out of loop
    }

    password = (num1+num2+num3);        // Set variable to sum of three entries

    if (password != 258)                // If the user entry does not match the passcode
        pw_flag = 1;                    // Flag set to 1

    if (password == 258)                // If the user entry matches the passcode
        pw_flag = 0;                    // Flag set to 0
}

/****| Port5_IRQHandler | *****************************************
* Brief: This function gives instruction to Port 5 when an
* interrupt is detected. When the push button on P5.2 is pressed,
* the DC motor stops. When the push button on P5.0 is pressed,
* the RBG LED toggles on/off.
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void PORT5_IRQHandler(void)
{
        if (P5->IFG & 0x04)             // If P5.0 or P5.2 had an interrupt, call debounce function
            Debounce_Switch();

    while (stop_flag == 1)
    {
        if (P5->IFG & BIT2)             // If P5.2 had an interrupt
        {
            TIMER_A0->CCR[1] = 0;       // Set DC motor duty cycle to 0
            P5->IFG &= ~BIT2;           // Reset the interrupt flag
        }

        if (P5->IFG & BIT0)             // If P5.0 had an interrupt
        {
            if (red_flag == 0)          // If red LED is ON
            {
                TIMER_A2->CCR[2] = 0;   // Turn red LED OFF
                red_flag = 1;           // Switch flag
            }

            else if (red_flag == 1)     // If red LED is OFF
            {
                TIMER_A2->CCR[2] = red_bright;  // Red LED to previous brightness
                red_flag = 0;           // Switch flag
            }

            if (green_flag == 0)        // If green LED is ON
            {
                TIMER_A2->CCR[3] = 0;   // Turn green LED OFF
                green_flag = 1;         // Switch flag
            }

            else if (green_flag == 1)   // If green LED is OFF
            {
                TIMER_A2->CCR[3] = green_bright;  // Green LED to previous brightness
                green_flag = 0;         // Switch flag
            }

            if (blue_flag == 0)         // If blue LED is ON
            {
                TIMER_A2->CCR[1] = 0;   // Turn blue LED OFF
                blue_flag = 1;          // Switch flag
            }

            else if (blue_flag == 1)    // If blue LED is OFF
            {
                TIMER_A2->CCR[1] = blue_bright;  // Blue LED to previous brightness
                blue_flag = 0;          // Switch flag
            }
            stop_flag = 0;              // Set flag back to 0
            P5->IFG &= ~BIT0;           // Clear all flags
        }
    }
}

/****| Debounce_Switch | *************************************
* Brief: This function checks to see if a button is pushed to
* deal with switch bounce
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void Debounce_Switch(void)
{
    stop_flag = 0;                      // Set flag to 0

    if ((P5->IN & 0x04) != 0x04)        // If a button is pushed
    {
        SysTick_delay_ms(10);           // Pause for switch bounce
        if ((P5->IN & 0x04) != 0x04)    // Check if button is still pushed
            stop_flag = 1;              // If button is pushed, set flag to 1
    }
}

/****| Red_LED | *********************************************
* Brief: This function sets the brightness of the red LED
* based on user input from the keypad
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void Red_LED(void)
{
    int num1 = 0, num2 = 0, num3 = 0, brightness = 0;

    // TAKING IN FIRST NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num1 = num*100;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    // TAKING IN SECOND NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num2 = num*10;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    // TAKING IN THIRD NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num3 = num;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    brightness = (num1+num2+num3);  // Set brightness to sum of three entries

    if (brightness > 100)           // If brightness is greater than 100, set to 100
        brightness = 100;

    brightness *= 300;              // Multiply by 1/100th of period (30,000)
    red_bright = brightness;

    if (red_bright != 0)            // If red LED is on, set flag to 0 for toggle function
        red_flag = 0;

    if (red_bright == 0)            // If red LED is off, set flag to 1 for toggle function
        red_flag = 1;

    TIMER_A2->CCR[2] = brightness;  // Set duty cycle to user entry
}

/****| Green_LED | ******************************************
* Brief: This function sets the brightness of the green LED
* based on user input from the keypad
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void Green_LED(void)
{
    int num1 = 0, num2 = 0, num3 = 0, brightness = 0;

    // TAKING IN FIRST NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num1 = num*100;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    // TAKING IN SECOND NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num2 = num*10;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    // TAKING IN THIRD NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num3 = num;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    brightness = (num1+num2+num3);  // Set brightness to sum of three entries

    if (brightness > 100)           // If brightness is greater than 100, set to 100
        brightness = 100;

    brightness *= 300;              // Multiply by 1/100th of period (30,000)
    green_bright = brightness;

    if (green_bright != 0)          // If green LED is on, set flag to 0 for toggle function
        green_flag = 0;

    if (green_bright == 0)          // If green LED is off, set flag to 1 for toggle function
        green_flag = 1;

    TIMER_A2->CCR[3] = brightness;  // Set duty cycle to user entry
}

/****| Blue_LED | *********************************************
* Brief: This function sets the brightness of the blue LED
* based on user input from the keypad
* param:
* (void) data: N/A
* return:
* N/A
*************************************************************/
void Blue_LED(void)
{
    int num1 = 0, num2 = 0, num3 = 0, brightness = 0;

    // TAKING IN FIRST NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num1 = num*100;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    // TAKING IN SECOND NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num2 = num*10;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    // TAKING IN THIRD NUMBER
    flag = 1;

    while (flag)
    {
        LCD_Backlight();            // Continuously checking for LCD backlight adjustments
        pressed = keypad_read();    // Continues to read keypad until a press is detected
        if (pressed)
        {
            SysTick_delay_ms(10);   // Delay

            if (pressed)            // Checking if button is still pressed to deal with switch bounce
            {
                if (num == 10 | num == 11 | num == 12)
                {
                    num = 0;        // Set num to zero if *, 0, or # are pressed
                }
                num3 = num;
                flag = 0;           // Reset flag to break out of the loop
            }
        }
    }

    brightness = (num1+num2+num3);  // Set brightness to sum of three entries

    if (brightness > 100)           // If brightness is greater than 100, set to 100
        brightness = 100;

    brightness *= 300;              // Multiply by 1/100th of period (30,000)
    blue_bright = brightness;

    if (blue_bright != 0)           // If blue LED is on, set flag to 0 for toggle function
        blue_flag = 0;

    if (blue_bright == 0)           // If blue LED is off, set flag to 1 for toggle function
        blue_flag = 1;

    TIMER_A2->CCR[1] = brightness;  // Set duty cycle to user entry
}
