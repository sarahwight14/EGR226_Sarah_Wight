/**************************************************************************************
*                                Menus_Library.c
*                                  Sarah Wight
*                         EGR226      Date: April 10, 2021
*
*  This is a library for the menus that will be displayed on the LCD.
*
*  All functions are briefly described in their comment blocks.  The /// notation makes
*  it so the function description block is visible when you hovering over a function call
*  in any file (this feature is called Intellisense).
*
*
* **************************************************************************************/

#include "msp.h"
#include <LCD_Library.h>

/// /****| Main_Menu | *****************************************
/// * Brief: This function displays the Main Menu options to
/// * the LCD screen
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Main_Menu(void)
{
    lcdSetText("   Main Menu    ", 0, 0);
    lcdSetText("[1] Door        ", 0, 1);
    lcdSetText("[2] Motor       ", 0, 2);
    lcdSetText("[3] Lights     ", 0, 3);
}

/// /****| Door_Menu | *****************************************
/// * Brief: This function displays the Door Menu options to
/// * the LCD screen
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Door_Menu(void)
{
    lcdClear();                             // Clear display
    lcdSetText("   Door Menu    ", 0, 0);
    lcdSetText("[1] Open Door   ", 0, 1);
    lcdSetText("[2] Close Door  ", 0, 2);
    lcdSetText("[3] Unarm Door  ", 0, 3);
}

/// /****| Motor_Menu | *****************************************
/// * Brief: This function displays the Motor Menu options to
/// * the LCD screen
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Motor_Menu(void)
{
    lcdClear();                             // Clear display
    lcdSetText("   Motor Menu   ", 0, 0);
    lcdSetText("Enter a speed   ", 2, 1);
    lcdSetText("from 1-9        ", 4, 2);
}

/// /****| Lights_Menu | *****************************************
/// * Brief: This function displays the Main Menu options to
/// * the LCD screen
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Lights_Menu(void)
{
    lcdClear();                             // Clear display
    lcdSetText("Lights Menu", 3, 0);
    lcdSetText("[1] Red Light", 0, 1);
    lcdSetText("[2] Green Light", 0, 2);
    lcdSetText("[3] Blue Light", 0, 3);
}

/// /****| Arm_Menu | *****************************************
/// * Brief: This function displays the Arm Menu options to
/// * the LCD screen
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Arm_Menu(void)
{
    lcdClear();                             // Clear display
    lcdSetText("Please Enter the ", 0, 1);
    lcdSetText("3 digit code to", 0, 2);
    lcdSetText("lock or unlock ", 1, 3);
}

/// /****| Red_Menu | *****************************************
/// * Brief: This function displays the Red LED Brightness
/// * Menu to the LCD screen
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Red_Menu(void)
{
    lcdClear();                             // Clear display
    lcdSetText("RED LED     ", 4, 0);
    lcdSetText("Please enter a ", 1, 1);
    lcdSetText("Brightness from", 0, 2);
    lcdSetText("1-100      ", 5, 3);
}

/// /****| Green_Menu | *****************************************
/// * Brief: This function displays the Green LED Brightness
/// * Menu to the LCD screen
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Green_Menu(void)
{
    lcdClear();                             // Clear display
    lcdSetText("GREEN LED     ", 3, 0);
    lcdSetText("Please enter a ", 1, 1);
    lcdSetText("Brightness from", 0, 2);
    lcdSetText("1-100      ", 5, 3);
}

/// /****| Blue_Menu | *****************************************
/// * Brief: This function displays the Blue LED Brightness
/// * Menu to the LCD screen
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Blue_Menu(void)
{
    lcdClear();                             // Clear display
    lcdSetText("BLUE LED     ", 3, 0);
    lcdSetText("Please enter a ", 1, 1);
    lcdSetText("Brightness from", 0, 2);
    lcdSetText("1-100      ", 5, 3);
}




