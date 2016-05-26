/*
level.h
AUTHOR: Jonathan Padilla jpadi004@ucr.edu
*/
#ifndef __level_h__
#endif	__level_h__
#include "bit.h"

#define RED 1 //primary tile color
#define BLUE 2//primary water/insta death color
#define GREEN 3//enemy color
#define PURPLE 4// boss color
#define EXIT_COLOR 5//entry/exit color (teal)
#define TILE 6//tile(red) which is to be used to determine if simon should land or fall
#define YELLOW 7//breakable objects color whip
#define TEAL 8 //whip color/projectile color
#define WHITE 9 //torch color
#define WHIP 10 // whip(white)
/*
#define _7 0x01 //bottom row
#define _6 0x02
#define _5 0x04
#define _4 0x08 //middle row from bottom
#define _3 0x10
#define _2 0x20
#define _1 0x40
#define _0 0x80 //top row
*/

//used to store color value at location
//used for render function to reduce # of times accessing the array
unsigned char player_HP;// = 0;
unsigned char loc_val;// = 0;
unsigned char crouched;// = 0;
unsigned char moving_left;// = 0;
unsigned char moving_right;// = 0;
unsigned char in_air;// = 0;
unsigned char has_moved;// = 0;
unsigned char cur_bit;// = 0;
unsigned char cur_level;// = 1;
unsigned char level_size_index;// = 0;
unsigned char start_render;// = 0;
unsigned char end_render;// = 8;
unsigned char whip_loc;// = 0;
unsigned char whip_loc2;// = 0;
unsigned char whip_loc3;// = 0;
unsigned char whipped;// = 0; //used to determine when you can attack again
signed char pos_x;// = 1; //used to determine Simon's position in x direction on map
signed char pos_y;// = 6; //used to determine Simon's position in y direction on map
unsigned long to_renderR;// = 0xFFFFFF;
unsigned long to_renderG;// = 0xFFFFFF;
unsigned long to_renderB;// = 0xFFFFFF;
unsigned long to_render;// = 0xFFFFFF;
unsigned long active_blue[8];// = {0,0,0,0,0,0,0,0};//current set of blue to render
unsigned long active_green[8];// = {0,0,0,0,0,0,0,0};//current set of green to render
unsigned long active_red[8];// = {0,0,0,0,0,0,0,0}; //current set of red to render
unsigned char active_row[8];//
	

//used to determine new range to render
//length x is the length of the map currently located in
//ADD FLAG FOR BOSS MODE
void calc_render_range(unsigned char length_x);
//0x00FFFF
//first 16 bits control blue lines [17-24]
//0xFF00FF
//middle 16 bits control red lines [8-16]
//0xFFFF00
//right bits control green lines[0-7]

//used to generate array of red to be rendered
//call after calc_render_range
void calc_render();

//movement and actions
void move_left();
void move_right();
void crouch();
void fall();
void jump();
void forward_jump();
void whip();
void retract_whip();
void subweapon();

//movement between levels
void next_level();
//void prev_level();
void reset();

#endif