/**************************************************************************************
*                                Init_Library.h
*                                  Sarah Wight
*                         EGR226      Date: April 10, 2021
*
*  This is a library for the initialization functions.
*
*  All functions are briefly described in their comment blocks.  The /// notation makes
*  it so the function description block is visible when you hovering over a function call
*  in any file (this feature is called Intellisense).
*
*
* **************************************************************************************/

#ifndef INIT_LIBRARY_H_
#define INIT_LIBRARY_H_

#include "msp.h"
#include <stdio.h>

/********************** Function Prototypes *****************************
************************************************************************/
void Servo_Init(void);          // Initialization function for servo motor & on board LEDs
void DC_Init(void);             // Initialization function for DC motor & push button
void RGB_Init(void);            // Initialization function for RBG LED & push button
void Backlight_Init(void);      // Initialization function for pins controlling LCD backlight
/********************** Function Prototypes *****************************
************************************************************************/


#endif /* INIT_LIBRARY_H_ */
