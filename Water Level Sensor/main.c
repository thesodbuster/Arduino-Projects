#include <msp430fr6989.h>
#define redLED BIT0 // P1.0 is redLED
#define water BIT0  // P4.0 is water sensor input

/**
 * main.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // Enable GPIO pins

	// Configure red LED at bit 0 to OFF and input
	P1DIR |= redLED;
	P1OUT &= ~redLED;

	P4DIR &= ~water;    // Configure pin 4 for input
	P4REN = water;      // Enable pull up/down resistor for P4.0
	P4OUT = water;      // Select pull up resistor for P4.0

	// Main loop
	for(;;)
	{
	    if (!(P4IN & water)) // wait until pin reads LOW
	    {
	        // Turn red LED ON
	        P1OUT |= redLED;
	    } else {
	        // Turn red LED OFF
	        P1OUT &= ~redLED;
	    }

	}

}
