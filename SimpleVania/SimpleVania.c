/*
SimpleVania.c
AUTHOR: Jonathan Padilla jpadi004@ucr.edu
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "bit.h"
#include "timer.h"
#include <stdio.h>
#include "game_objects.h"
#include "io.c"
#include "level.c"
//#include "controller.c"
/*
Clock Cycle	Button Reported
1	B [0]
2	Y [1]
3	Select [2]
4	Start [3]
5	Up on D-pad [4]
6	Down on D-pad [5]
7	Left on  D-pad [6]
8	Right on D-pad [7]
9	A [8]
10	X [9]
11	L [10]
12	R [11]
13-16	none (always high)
*/
unsigned char buttons[16];
void getControllerData()
{
	unsigned char cCopy;     //Copy of the current input via controller
	int i = 0;               //Index
	PORTC = 0x03;            //Setting the clock and the latch to high. Sets value of buttons to Shift Register
	for(i = 0; i < 16; i++)  //For loop gathering data
	{
		PORTC = 0x01;                     //Setting clock to high in order to shift data over one spot.
		cCopy = ((~PINC) >> 2) & 0x01;    //Buttons have pull-up resistors, so logical NOT values and takes data from PINC 2
		buttons[i] = cCopy;             //Saving values to global vector
		//Saving values to global vector
		PORTC = 0x00;                     //Setting everything to low so that rising edge can trigger shift
	}
	PORTC = 0x01;                         //Setting clock to high when not in use

};


//--------Find GCD function --------------------------------------------------
unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
//--------End find GCD function ----------------------------------------------

void transmit_dataA(unsigned char data) {
	int i;
	for (i = 0; i < 8 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTA = 0x08;
		// set SER = next bit of data to be sent.
		PORTA |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTA |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTA |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTA = 0x00;
}

void transmit_dataB(unsigned long data) {
	int i;
	for (i = 0; i < 24 ; ++i) {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTB = 0x08;
		// set SER = next bit of data to be sent.
		PORTB |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTB |= 0x02;
	}
	// set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
	PORTB |= 0x04;
	// clears all lines in preparation of a new transmission
	PORTB = 0x00;
}
//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;
//----------------------------------------------------------------

//---------------------DEFINE MAIN CHARACTER --------------
Simon Belmont = {4,"DAGGER",1};
bat_boss not_drac = {4,"PURPLE"};

unsigned char r = 0;
unsigned char g = 0;
unsigned char b = 0;
enum RED_states{red_display,green_display,blue_display};
int render_SM(int state)
{
	calc_render_range(lvl_sizes[level_size_index]);
	switch(state)//transitions
	{	
	case red_display: //red -> green
	state = green_display;
	break;
	
	case green_display: //green -> blue
	state = blue_display;
	break;
	
	case blue_display: // blue -> red
	state = red_display;
	if(has_moved)
	{
	 calc_render();
	 //calc_renderRED();	
	}
	break;
	
	default: 
	state = red_display;
	calc_render();
	//calc_renderRED();
	break;
	}
	
	switch(state)//actions
	{
		case red_display:
		if(r > 7)
		{
			r = 0;
		}
		else
		{
			//transmit_dataB(active_red[r]);
			transmit_dataA(active_row[r]);//{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};//goes from top to bottom
			transmit_dataB(active_red[r]);//column values red
			r++;
			
		}
		break;
		
		case green_display:
		if(g > 7)
		{
			g = 0;
		}
		else
		{
			transmit_dataA(active_row[g]);//{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};//goes from top to bottom
			transmit_dataB(active_green[g]);//column values green
			g++;
		}
		break;
		
		case blue_display:
		if(b > 7)
		{
			b = 0;
		}
		else
		{
			transmit_dataA(active_row[b]);
			transmit_dataB(active_blue[b]);
			b++;
		}
		//state = red_display;
		break;
		
		default:
		//do nothing
		break;
	}
	return state;
}

/*
Clock Cycle	Button Reported
1	B
2	Y
3	Select
4	Start
5	Up on D-pad
6	Down on D-pad
7	Left on  D-pad
8	Right on D-pad
9	A
10	X
11	L
12	R
13-16	none (always high)
*/

enum control_states {control_wait, control_left, control_right, control_up, control_down, 
					 control_B, control_X, control_Y, control_BY,control_rightBY,
					 control_leftBY,control_downY, control_BX, control_select, control_start,
					 control_jumpanim1, control_jumpanim2, control_jumpanim3, control_rightB, control_leftB
					 ,control_leftY};
//state machine calls on getControllerData function to receive input from controller
//getControllerData modifies the buttons vector
//after certain buttons are pressed
//it will alter the arrays which contain what needs to be rendered'
int control_SM(int state)
{
	getControllerData();
	// === Transitions === //////////////////////
	switch (state)
	{
		//5	Up on D-pad //6	Down on D-pad //7	Left on  D-pad //8	Right on D-pad
		case control_wait:
		if(buttons[2])
		{			
			state = control_select;
			break;
		}
		if(!in_air)
		{
				//JUMPING
				if(buttons[0])//b button
				{
					state = control_B;
					//OTHER POSSIBLE JUMPING ACTIONS
					if(buttons[6])//left also pressed (left jump)
					{
						state = control_leftB;
					}
					if(buttons[7])//right and B (right jump)
					{
						state = control_rightB;
					}
					if(buttons[9])// B and X pressed (JUMP AND SUBWEAPON)
					{
					  state = control_BX;
					}
					if(buttons[1])//jump whip (B Y)
					{
						state = control_BY;
					}
				}
				else if(buttons[1])//y button pressed (WHIP HAS PRIORITY OVER MOVEMENT)
				{
				  state = control_Y;
				  if(buttons[0]) //jump whip
				  {
					  state = control_BY;
				  }
				  else if(buttons[5])//down whip
				  {
					  state = control_downY;
				  }
				  else if(buttons[6])//left whip
				  {
					  state = control_leftY;
				  }
				}
				else if(buttons[9])//x button pressed (SUB WEP HAS PRIORITY OVER MOVEMENT)
				{
				  state = control_X;
				}
				else if(buttons[6])//left button pressed
				{
					state = control_left;
				}
				else if(buttons[7])//right
				{
					state = control_right;
				}
				else if(buttons[4])//up
				{
					state = control_up ;
				}
				else if(buttons[5])//down
				{
					state = control_down;
				}
		}
		else
		{
			//0	B
			//1	Y
			//9 X
			//Only 2 actions could be performed in the air at any time
			//this is limited to using the whip(Y) or using the subwep(X)
			if(buttons[1])//y button pressed
			{
				state = control_Y;
			}
			else if(buttons[9])//x button pressed
			{
			    state = control_X;	
			}
		}
		
		//do nothing otherwise
		break;
		///////DPAD BASIC MOVEMENT/////////
		case control_left:
		if(buttons[0])//left also pressed (left jump)
		{
		  state = control_leftB;
		}
		else if(buttons[1])//whip Y
		{
			moving_left = 1;
			moving_right = 0;
			state = control_leftY;
		}
		else if(buttons[6])
		{
			state = control_left;
		}
		else
		{
			state = control_wait;
		}
		break;
		
		case control_right:
		if(buttons[0] && !in_air)
		{
			state = control_rightB;
		}
		else if(buttons[1])//whip Y
		{
			state = control_Y;
		}
		else if(buttons[7])
		{
			state = control_right;
		}
		else{
			state = control_wait;
		}
		break;
		
		case control_up:
		if(buttons[4])
		{
			state = control_up; //UP doesn't do anything
		}
		else
		{
			state = control_wait;
		}
		break;
		
		case control_down:
		if(buttons[5] && !buttons[1])//holding down but no Y pressed
		{
			state = control_down;
		}
		else if(buttons[1])
		{
			crouched = 1;
			in_air = 0;
			state = control_downY;
		}
		else
		{
			
			state = control_wait;
		}
		break;
		////////////END DPAD///////////////
		
		//////////START B Y X /////////////////
		case control_B:
		//state = control_jumpanim1;
		state = control_wait;
		break;
		
		case control_leftB:
		state = control_jumpanim1;
		break;
		
		case control_rightB:
		state = control_jumpanim1;
		break;
		case control_jumpanim1:
		state = control_jumpanim2;
		
		break;
		
		case control_jumpanim2:
		state = control_jumpanim3;
		//state = control_wait;
		break;
		
		case control_jumpanim3:
		state = control_wait;
		break;
		case control_X: 
		state = control_wait;
		break;
		 
		case control_Y: 
		retract_whip();
		state = control_wait;
		break;
		
		case control_BY:
		retract_whip();
		state = control_wait;
		break;
		
		case control_rightBY:
		retract_whip();
		state = control_wait;
		break;
		
		case control_leftBY:
		retract_whip();
		state = control_wait;
		break;
		
		case control_leftY:
		retract_whip();
		state = control_wait;
		break;
		
		case control_downY:
		retract_whip();
		state = control_wait;
		break;
		
		case control_BX:
		state = control_wait;
		break;
		
		case control_start:
		
		break;
		
		case control_select:
		
		
		state = control_wait;
		break;
		
		default:
		state = control_wait;
		break;
	}
/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////ACTIONS//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
	switch (state)//actions
	{
		case control_wait: //stationary so see if blocks underneath are empty
		//default to standing position
		if(!in_air)
		{
			has_moved = 0;
		}
		//has_moved = 0;
		fall();	
		break;
		
		//// D PAD //////////////////
		case control_left:
		moving_left = 1;
		move_left();
		break;
		
		case control_right:
		move_right();
		break;
		
		case control_up:
		//DOES NOTHING
		break;
		
		case control_down:
		crouch();
		break;
		
		//////////END D PAD///////////////
		case control_B:
		   jump();
		   break;
		
		case control_rightB:
		moving_right = 1;
		moving_left = 0;
		jump();
		break;
		
		case control_leftB:
		moving_left = 1;
		moving_right = 0;
		jump();
		break;
		
		case control_jumpanim1:
		forward_jump();
		break;
		
		case control_jumpanim2:
		forward_jump();
		break;
		
		case control_jumpanim3:
		forward_jump();
		break;
		
		case control_X:
		
		break;
		
		///////////WHIP/////////////
		case control_Y:
		if(!whipped)
		{
			whip();	
		}
		whipped = 1;
		break;
		
		case control_BY:
		crouched = 1;
		if(!whipped)
		{
			whip();
		}
		whipped = 1;
		break;
		
		case control_rightBY:
		moving_left = 0;
		moving_right = 1;
		crouched = 1;
		if(!whipped)
		{
			whip();
		}
		whipped = 1;
		break;
		
		case control_leftBY:
		moving_left = 1;
		moving_right = 0;
		crouched = 1;
		if(!whipped)
		{
			whip();
		}
		whipped = 1;
		break;
		
		case control_leftY:
		moving_left = 1;
		moving_right = 0;
		crouched = 1;
		if(!whipped)
		{
			whip();
		}
		whipped = 1;
		break;
		
		case control_downY:
		crouched = 1;
		if(!whipped)
		{
			whip();
		}
		whipped = 1;
		break;
		
		case control_BX:
		
		break;
		
		case control_start:
		
		break;
		
		case control_select:
		reset();
		break;
		
		default: break;
	}
	return state;
}

enum whipD_states {delay_whip};
int whip_delay_SM(int state)
{
	switch(state)
	{
		case delay_whip:
		state = delay_whip;
		break;
		default:
		state = delay_whip;
		break;
	}
	
	switch(state)
	{
		case delay_whip:
		if(whipped)
		{
			whipped = 0;
		}
		break;
		default:
		whipped = 0;
		break;
	}
	
	return state;
}

enum LCD_states{LCD_display};
int LCD_sm(int state)
{
	switch(state)
	{
		case LCD_display:
		state = LCD_display;
		break;
		default:
		state = LCD_display;
		break;
	}
	
	switch(state)
	{
		case LCD_display:
		if(player_HP == 0)
		{
			LCD_DisplayString(17, "YOU LOSE!");
			player_HP = 4;
		}
		else
		{
				//LCD_DisplayString(1, "")
				LCD_DisplayString(17,"HP:");
				LCD_Cursor(21);
				LCD_WriteData(player_HP+'0');
		}
		break;
		default:
		//do nothing
		break;
	}
	return state;
}
// Implement scheduler code from PES.
int main()
{
	DDRB = 0xFF; PORTB = 0x00; //output
	DDRA = 0xFF; PORTA = 0x00; //output 
	DDRC = 0x03; PORTC = 0xFC; //input/output
	DDRD = 0xFF; PORTD = 0x00; //output
	
	LCD_init();
	//char* whip_stat = strcat("WHIP LVL: ", "1");
	//const unsigned char* test = Belmont.color;
	const unsigned char* mes = ">>SIMPLEVANIA<<";
	LCD_DisplayString(1, mes);
	//LCD_DisplayString(17, "SUB: Dagger");
	//LCD_DisplayString()
	
	cur_level = 1;
	level_size_index = 0;
	calc_render_range(lvl_sizes[level_size_index]);
	moving_right = 1;
	crouched = 0;
	in_air = 0;
	whipped = 0;
	
	// Period for the tasks
	unsigned long int renderR_calc = 1;
	unsigned long int whip_calc = 500;
	unsigned long int control_calc = 75;
	unsigned long int LCD_calc = 1000;

	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(renderR_calc, control_calc);
	tmpGCD = findGCD(tmpGCD, whip_calc);
	tmpGCD = findGCD(tmpGCD, LCD_calc);
	//tmpGCD = findGCD(tmpGCD, SMTick4_calc);

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int renderR_period = renderR_calc/GCD;
	unsigned long int whip_period = whip_calc/GCD;
	unsigned long int control_period = control_calc/GCD;
	unsigned long int LCD_period = LCD_calc/GCD;
	//unsigned long int SMTick3_period = SMTick3_calc/GCD;
	//unsigned long int SMTick4_period = SMTick4_calc/GCD;

	//Declare an array of tasks
	//static task task1, task2, task3;// task2, task3, task4;
	//task *tasks[] = { &task1, &task2, &task3};//, &task2, &task3, &task4 };
	static task task1, task2, task3, task4;// task2, task3, task4;
	task *tasks[] = { &task1, &task2 , &task3 , &task4};//, &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = -1;//Task initial state.
	task1.period = renderR_period;//Task Period.
	task1.elapsedTime = renderR_period;//Task current elapsed time.
	task1.TickFct = &render_SM;//Function pointer for the tick.
	
	
	// Task 2
	
	task2.state = -1;//Task initial state.
	task2.period = whip_period;//Task Period.
	task2.elapsedTime = whip_period;//Task current elapsed time.
	task2.TickFct = &whip_delay_SM;//Function pointer for the tick.
	
	// Task 3
	task3.state = -1;//Task initial state.
	task3.period = control_period;//Task Period.
	task3.elapsedTime = control_period;//Task current elapsed time.
	task3.TickFct = &control_SM;//Function pointer for the tick.

	// Task 3
	//task3.state = -1;//Task initial state.
	//task3.period = SMTick3_period;//Task Period.
	//task3.elapsedTime = SMTick3_period; // Task current elasped time.
	//task3.TickFct = &SMTick3; // Function pointer for the tick.

	// Task 4
	task4.state = -1;//Task initial state.
	task4.period = LCD_period;//Task Period.
	task4.elapsedTime = LCD_period; // Task current elasped time.
	task4.TickFct = &LCD_sm; // Function pointer for the tick.

	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();

	unsigned short i; // Scheduler for-loop iterator
	while(1) {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	// Error: Program should not exit!
	return 0;
}