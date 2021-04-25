/**************************************************************************************
*                                Keypad_Library.h
*                                  Sarah Wight
*                         EGR226      Date: April 10, 2021
*
*  This is a library for the keypad.
*
*  All functions are briefly described in their comment blocks.  The /// notation makes
*  it so the function description block is visible when you hovering over a function call
*  in any file (this feature is called Intellisense).
*
*
* **************************************************************************************/

#ifndef KEYPAD_LIBRARY_H_
#define KEYPAD_LIBRARY_H_

#include "msp.h"
#include <stdio.h>

/********************** Function Prototypes *****************************
************************************************************************/
void keypad_init(void);
uint8_t keypad_read(void);
void print_keys(void);
/********************** Function Prototypes *****************************
************************************************************************/


/************************* Global Flags ********************************
************************************************************************/
uint8_t num, pressed;
/************************* Global Flags ********************************
************************************************************************/


#endif /* KEYPAD_LIBRARY_H_ */
