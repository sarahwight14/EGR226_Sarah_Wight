/**************************************************************************************
*                                Init_Library.c
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
****************************************************************************************/

#include "msp.h"

/// /****| DC_Init | *********************************************
/// * Brief: This function initializes pin P2.4 for Timer A, sets
/// * the period and mode of the DC motor, initializes a push
/// * button on pin P5.2, and enables interrupts for pin P5.2
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void DC_Init(void)
{
    // DC Motor Initialization - configure GPIO for PWM output
    P2->SEL0 |= BIT4;
    P2->SEL1 &= ~(BIT4);
    P2->DIR |= BIT4;

    TIMER_A0->CCR[0] |= 30000;      // PWM period (# cycles of clock)
    TIMER_A0->CCTL[1] |= 0xE0;      // CCR1 reset/set mode 7
    TIMER_A0->CCR[1] = 0;           // CCR1 PWM duty cycle
    TIMER_A0->CTL |= 0x0214;        // SMCLK, Up mode, clear TAR to start

    // Button Initialization
    P5->SEL1 &= ~BIT2;              // Configure Pin P5.2 as simple I/O
    P5->SEL0 &= ~BIT2;
    P5->DIR &= ~BIT2;               // Pin P5.2 set as input
    P5->REN |= BIT2;                // Pin P5.2 pull resistor enabled
    P5->OUT |= BIT2;
    P5->IES |= BIT2;                // Set pin interrupt to trigger when it goes from high to low
    P5->IE |= BIT2;                 // Enable interrupt for Pin P5.2
    P5->IFG &= ~BIT2;               // Clear P5.2 interrupt flags
}

/// /****| Servo_Init | *********************************************
/// * Brief: This function initializes pin P2.5 and P2.6 for Timer A,
/// * sets the period and mode of the servo motor and piezzo sounder,
/// * and initializes the red and green on board LEDs.
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Servo_Init(void)
{
    // Servo Initialization - configure GPIO for PWM output
    P2->SEL0 |= BIT5;
    P2->SEL1 &= ~(BIT5);
    P2->DIR |= BIT5;

    TIMER_A0->CCR[0] |= 30000;      // PWM period (# cycles of clock)
    TIMER_A0->CCTL[2] |= 0xE0;      // CCR2 reset/set mode 7
    TIMER_A0->CCR[2] = 9000;        // CCR2 PWM duty cycle
    TIMER_A0->CTL |= 0x0214;        // SMCLK, Up mode, clear TAR to start

    // Red LED Initialization
    P2->SEL1 &= ~BIT0;              // Configure P2.0 as simple I/O
    P2->SEL0 &= ~BIT0;
    P2->DIR |= BIT0;                // P2.0 set as output pin
    P2->OUT &= ~BIT0;

    // Green LED Initialization
    P2->SEL1 &= ~BIT1;              // Configure P2.1 as a simple I/O
    P2->SEL0 &= ~BIT1;
    P2->DIR |= BIT1;                // P2.1 set as output pin
    P2->OUT &= ~BIT1;

    // Sounder Initialization - configure GPIO for PWM output
    P2->SEL0 |= BIT6;
    P2->SEL1 &= ~(BIT6);
    P2->DIR |= BIT6;

    TIMER_A0->CCR[0] |= 30000;      // PWM period (# cycles of clock)
    TIMER_A0->CCTL[3] |= 0xE0;      // CCR2 reset/set mode 7
    TIMER_A0->CCR[3] = 0;           // CCR2 PWM duty cycle
    TIMER_A0->CTL |= 0x0214;        // SMCLK, Up mode, clear TAR to start
}

/// /****| RGB_Init | *********************************************
/// * Brief: This function initializes pins P5.6, P5.7, and P6.6
/// * for Timer A, sets the period and mode of the red, green, and
/// * blue LEDs, initializes a push button on pin P5.0, and enables
/// * interrupts for pin P5.0
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void RGB_Init(void)
{
    // Blue LED - configure GPIO for PWM output
    P5->SEL0 |= BIT6;
    P5->SEL1 &= ~(BIT6);
    P5->DIR |= BIT6;

    TIMER_A2->CCR[0] |= 30000;      // PWM period (# cycles of clock)
    TIMER_A2->CCTL[1] |= 0xE0;      // CCR1 reset/set mode 7
    TIMER_A2->CCR[1] = 0;           // CCR1 PWM duty cycle
    TIMER_A2->CTL |= 0x0214;        // SMCLK, Up mode, clear TAR to start

    // Red LED - configure GPIO for PWM output
    P5->SEL0 |= BIT7;
    P5->SEL1 &= ~(BIT7);
    P5->DIR |= BIT7;

    TIMER_A2->CCR[0] |= 30000;      // PWM period (# cycles of clock)
    TIMER_A2->CCTL[2] |= 0xE0;      // CCR2 reset/set mode 7
    TIMER_A2->CCR[2] = 0;           // CCR2 PWM duty cycle
    TIMER_A2->CTL |= 0x0214;        // SMCLK, Up mode, clear TAR to start

    // Green LED - configure GPIO for PWM output
    P6->SEL0 |= BIT6;
    P6->SEL1 &= ~(BIT6);
    P6->DIR |= BIT6;

    TIMER_A2->CCR[0] |= 30000;      // PWM period (# cycles of clock)
    TIMER_A2->CCTL[3] |= 0xE0;      // CCR3 reset/set mode 7
    TIMER_A2->CCR[3] = 0;           // CCR3 PWM duty cycle
    TIMER_A2->CTL |= 0x0214;        // SMCLK, Up mode, clear TAR to start

    // Button Initialization
    P5->SEL1 &= ~BIT0;              // Configure Pin P5.0 as simple I/O
    P5->SEL0 &= ~BIT0;
    P5->DIR &= ~BIT0;               // Pin P5.0 set as input
    P5->REN |= BIT0;                // Pin P5.0 pull resistor enabled
    P5->OUT |= BIT0;
    P5->IES |= BIT0;                // Set pin interrupt to trigger when it goes from high to low
    P5->IE |= BIT0;                 // Enable interrupt for Pin P5.0
    P5->IFG &= ~BIT0;               // Clear P5.0 interrupt flags
}

/// /****| Backlight_Init | *********************************************
/// * Brief: This function initializes pin P7.4 for Timer A, sets
/// * the period and mode of the LCD backlight, configures pin P5.5
/// * for ADC, and enables ADC
/// * param:
/// * (void) data: N/A
/// * return:
/// * N/A
/// *************************************************************/
void Backlight_Init(void)
{
    // LCD Backlight - configure GPIO for PWM output
    P7->SEL0 |= BIT4;
    P7->SEL1 &= ~(BIT4);
    P7->DIR |= BIT4;

    TIMER_A1->CCR[0] |= 1000;       // PWM period (# cycles of clock)
    TIMER_A1->CCTL[4] |= 0xE0;      // CCR1 reset/set mode 7
    TIMER_A1->CCR[4] |= 500;        // CCR1 PWM duty cycle
    TIMER_A1->CTL |= 0x0214;        // SMCLK, Up mode, clear TAR to start

    // ADC setup
    ADC14->CTL0 = 0x00000010;       // Power on and disabled during configuration
    ADC14->CTL0 |= 0x04D80300;      // S/H pulse mode, MCLCK, 32 sample clocks, sw trigger, /4 clock divide
    ADC14->CTL1 = 0x00000030;       // 14-bit resolution

    ADC14->MCTL[5] = 0;             // A0 input, single ended, vref=avcc

    P5->SEL1 |= BIT5;               // Configure pin 5.5 for AO
    P5->SEL0 |= BIT5;

    ADC14->CTL1 |= 0x00050000;      // Start converting at mem reg 5
    ADC14->CTL0 |= 2;               // Enable ADC after configuration

    TIMER_A1->CCR[4] = 500;         // Duty cycle to 50% to start
}

