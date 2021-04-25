/**************************************************************************************
*                                Menus_Library.h
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

#ifndef MENUS_LIBRARY_H_
#define MENUS_LIBRARY_H_

#include "msp.h"
#include <stdio.h>

/********************** Function Prototypes *****************************
************************************************************************/
void Main_Menu(void);           // Displays the Main Menu to the LCD
void Door_Menu(void);           // Displays the Door Menu to the LCD
void Motor_Menu(void);          // Displays the Motor Menu to the LCD
void Lights_Menu(void);         // Displays the Lights Menu to the LCD
void Arm_Menu(void);            // Displays the Arm Menu to the LCD
void Red_Menu(void);            // Displays the Brightness Menu for the red LED
void Green_Menu(void);          // Displays the Brightness Menu for the green LED
void Blue_Menu(void);           // Displays the Brightness Menu for the blue LED
/********************** Function Prototypes *****************************
************************************************************************/

#endif /* MENUS_LIBRARY_H_ */
