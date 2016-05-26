#ifndef __CONTROLLER__
#define __CONTROLLER__
/*
 * CONTROLLER.c
 *
 *  Author: Johnny Do
 */ 

#include <avr/io.h> 

unsigned char bitVector[16]; //Vector with the value of the buttons depressed

unsigned char* getControllerData()
{
	unsigned char cCopy;     //Copy of the current input via controller
	int i = 0;               //Index
	PORTC = 0x03;            //Setting the clock and the latch to high. Sets value of buttons to Shift Register
	for(i = 0; i < 16; i++)  //For loop gathering data
	{
		PORTC = 0x03;                     //Setting clock to high in order to shift data over one spot.
		cCopy = ((~PINC) >> 2) & 0x01;    //Buttons have pull-up resistors, so logical NOT values and takes data from PINC 2
		bitVector[i] = cCopy;             //Saving values to global vector
							              //Saving values to global vector
		PORTC = 0x00;                     //Setting everything to low so that rising edge can trigger shift
	}
	PORTC = 0x01;                         //Setting clock to high when not in use

return bitVector;
};

/*
Clock Cycle	Button Reported
1	B
2	Y
3	Select
4	Start
5	Up on joypad
6	Down on joypad
7	Left on joypad
8	Right on joypad
9	A
10	X
11	L
12	R
13-16	none (always high)

Pin	Description	Wire Color
1	+5v	White
2	Data Clock	Yellow
3	Data Latch	Orange
4	Serial Data	Red
5	N/C	-
6	N/C	-
7	Ground	Brown
*/

#endif