/*
level.c
AUTHOR: Jonathan Padilla jpadi004@ucr.edu
*/
#include "bit.h"

//used to generate array of red to be rendered
//call after calc_render_range
void calc_renderRED();
void calc_renderGREEN();
void calc_renderBLUE();
//movement and actions
void move_left();
void move_right();
void crouch();
void fall();
void jump();
void forward_jump1();
void forward_jump2();
void whip();
void subweapon();
void reset();
//movement between levels
void next_level();
void prev_level();

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

//used to store color value at location
//used for render function to reduce # of times accessing the array
unsigned char num_purple = 0;
unsigned char player_HP = 4;
unsigned char loc_val = 0;
unsigned char crouched = 0;
unsigned char moving_left = 0;
unsigned char moving_right = 0;
unsigned char in_air = 0;
unsigned char has_moved = 0;
unsigned char cur_bit = 0;
unsigned char cur_level = 1;
unsigned char level_size_index = 0;
unsigned char start_render = 0;
unsigned char end_render = 8;
unsigned char whip_loc = 0;
unsigned char whip_loc2 = 0;
unsigned char whip_loc3 = 0;
unsigned char whipped = 0; //used to determine when you can attack again
signed char pos_x = 1; //used to determine Simon's position in x direction on map
signed char pos_y = 6; //used to determine Simon's position in y direction on map
unsigned long to_renderR = 0xFFFFFF;
unsigned long to_renderG = 0xFFFFFF;
unsigned long to_renderB = 0xFFFFFF;
unsigned long to_render = 0xFFFFFF;
unsigned long active_blue[8] = {0,0,0,0,0,0,0,0};//current set of blue to render
unsigned long active_green[8] = {0,0,0,0,0,0,0,0};//current set of green to render
unsigned long active_red[8] = {0,0,0,0,0,0,0,0}; //current set of red to render
unsigned char active_row[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};//goes from top to bottom
unsigned char level1[8][134] = {
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,5,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,5},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,9,0,9,0,0,0,0,0,0,0,6,0,0,0,0,0,0,5},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,6,6,6,0,0,6,6,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,4,0,0,6,4,0,0,0,6,6,6},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,0,0,6,6,6,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,6,6,0,0,0,6,6,6,0,0,0,6,6,6,0,0,0,0,0,0,6,6,3,0,3,0,6,6,6,6,0,0,0,0,0,4,4,6,0,6,6,0,0,0,6},
{0,7,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,5,0,9,0,0,0,0,9,0,0,0,9,0,3,0,9,0,0,0,9,0,3,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,6,6,6,6,6,6,6,6,6,6,6,3,0,0,0,0,0,0,0,0,3,6,6,6,0,0,0,0,7,6,6,0,9,0,0,9,0,0,9,0,0,0,0,0,4,4,6,0,0,0,0,0,0,6},
{0,7,0,9,0,0,0,0,9,0,0,0,0,9,0,0,0,0,9,0,0,0,0,9,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,6,6,6,6,6,6,6,6,6,6,6,6,6,3,0,0,0,0,0,0,0,0,3,6,6,6,0,0,0,0,7,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,6,0,0,0,0,0,0,6},
{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6}};
//---------------------LEVEL 1 ----------------------------///------------------------------------------------------ LEVEL 2 ------------------------------------------------------////------- LEVEL 3 TOP ----------------////------------------ LEVEL 4 ------------------------------------------//				
				//initialize 2D arrays with zero vectors
unsigned char level2[8][20] =   {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,6},
								 {0,9,7,0,9,0,0,9,0,0,0,9,0,0,0,0,0,0,0,6},
								 {0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,6,0},
								 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0},
								 {6,6,6,6,6,6,6,6,0,0,6,6,6,0,0,6,6,6,6,6},
								 {0,0,3,0,3,0,0,0,0,0,0,3,0,0,0,0,3,0,0,6},
								 {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,6,6,2},
								 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

//create array of levels;
//unsigned char lvl_sizes[] = {30, 60, 20 , 20 , 20, 24};
unsigned char lvl_sizes[] = {134, 20};


//used to determine new range to render
//length x is the length of the map currently located in
//ADD FLAG FOR BOSS MODE
void calc_render_range(unsigned char length_x)
{
	//determine range to render
	if(pos_x == 0)//if at left most position
	{
		start_render = 0;
		end_render = 8;
	}
	else if(pos_x+1 == length_x || pos_x+2 == length_x || pos_x+3 == length_x  || pos_x+4 == length_x  )//if at right most position
	{
		start_render = length_x-8;
		end_render = length_x;
	}
	else if(pos_x < 4)
	{
		start_render = 0;
		end_render = 8;
	}
	else //if(pos_x >= 4 && pos_x+4 <= length_x)
	{
			start_render = pos_x-3;
			end_render = pos_x+5;
	}
}

//0x00FFFF
//first 16 bits control blue lines [23-16]
//0xFF00FF
//middle 16 bits control red lines [15-8]
//0xFFFF00
//right bits control green lines [7-0]
//fills active_red array to properly display what is on screen
//fills active_blue array to properly display what is on screen
//fills active_green array to properly display what is on screen
void calc_render()
{
//reset values on to_render
 to_renderR = 0xFFFFFF;
 to_renderG = 0xFFFFFF;
 to_renderB = 0xFFFFFF;
	
 for(unsigned char y = 0; y < 8 ; y++)
 {
  cur_bit = 7;//reset bit to change
   for(unsigned char x = start_render; x < end_render ; x++)
   {
	  //set loc val dependent on level
	  if(cur_level == 1)
	  {
	   loc_val = level1[y][x];
	  }
	  else if(cur_level == 2)
	  {
	   loc_val = level2[y][x];  
	   //loc_val = level2[y][x];
  	  }
	 /*
	  else if(cur_level == 3)
	  {
		  loc_val = level3top[y][x];
	  }
	  else if(cur_level == 4)
	  {
		  loc_val = level3bot[y][x];
	  }
	  else if(cur_level == 5)
	  {
		  loc_val = level3top[y][x];
	  }
	  else if(cur_level == 6)
	  {
		  loc_val = level4[y][x];
	  }
	  */
	  //RED
	  if(loc_val == RED || loc_val == TILE || loc_val == YELLOW || loc_val == PURPLE || loc_val == WHITE || loc_val == WHIP)
	  {
	    to_renderR = SetLongREDBit(to_renderR,cur_bit,0);
	  }
	  //GREEN
	  if(loc_val == GREEN || loc_val == YELLOW || loc_val == TEAL || loc_val == EXIT_COLOR  || loc_val == WHITE || loc_val == WHIP)
	  {
	   to_renderG = SetLongGREENBit(to_renderG,cur_bit,0);	
      }
	  //BLUE
     if(loc_val == BLUE || loc_val == PURPLE || loc_val == TEAL || loc_val == EXIT_COLOR || loc_val == WHITE || loc_val == WHIP)
     {
       to_renderB = SetLongBLUEBit(to_renderB,cur_bit,0);
     }
	 
	 cur_bit--;//move to next bit to render
   }//inner for loop (x)
   //place values in respective arrays
   active_red[y] = to_renderR;
   active_green[y] = to_renderG;
   active_blue[y] = to_renderB;
   //reset long values
   to_renderR = 0xFFFFFF;
   to_renderG = 0xFFFFFF;
   to_renderB = 0xFFFFFF;
  }//outer loop y
}


//movement and actions
void move_left()
{
	moving_left = 1;
	moving_right = 0;
   if(cur_level == 1)
   {
	if(pos_x != 0 && level1[pos_y][pos_x-1] != TILE && level1[pos_y-1][pos_x-1] != TILE && level1[pos_y+1][pos_x] != 0 &&  level1[pos_y+1][pos_x] != EXIT_COLOR)
	{
		  if(level1[pos_y][pos_x-1] == GREEN)
		  {
			  player_HP--;
		  }
		  
		  if(player_HP == 0)
		  {
			  reset();
		  }
		has_moved = 1;
		level1[pos_y][pos_x] = 0;
		level1[pos_y-1][pos_x] = 0;
		pos_x--;
		level1[pos_y][pos_x] = YELLOW;
		level1[pos_y-1][pos_x] = YELLOW;
	}
	else if(level1[pos_y+1][pos_x-1] == EXIT_COLOR)
	{
	  next_level();
	}
	else if(level1[pos_y+1][pos_x] == 0)
	{
	  fall();
	}
   }
   if(cur_level == 2)
   {
	   if(pos_x != 0 && level2[pos_y][pos_x-1] != TILE && level2[pos_y-1][pos_x-1] != TILE && level2[pos_y+1][pos_x] != 0)
	   {
		   if(level2[pos_y][pos_x-1] == GREEN)
		   {
			   player_HP--;
		   }
		   
		   if(player_HP == 0)
		   {
			   reset();
		   }
		   has_moved = 1;
		   level2[pos_y][pos_x] = 0;
		   level2[pos_y-1][pos_x] = 0;
		   pos_x--;
		   level2[pos_y][pos_x] = YELLOW;
		   level2[pos_y-1][pos_x] = YELLOW;
	   }
	   else if(level2[pos_y+1][pos_x] == 0)
	   {
		fall();   
	   }
   }
   
}

void move_right()
{
		moving_left = 0;
		moving_right = 1;
	if(cur_level == 1)
	{
	 	//does not equal level length
	 	if(pos_x+1 != lvl_sizes[0] && level1[pos_y][pos_x+1] != TILE &&  level1[pos_y-1][pos_x+1] != TILE && level1[pos_y+1][pos_x] != 0)//cannot move through tile
	 	{
			 if(level1[pos_y][pos_x+1] == GREEN)
			 {
				 player_HP--;
			 }
			 if(player_HP == 0)
			 {
				 reset();
			 }
			 has_moved = 1;
		 	level1[pos_y][pos_x] = 0;
		 	level1[pos_y-1][pos_x] = 0;
		 	pos_x++;
		 	level1[pos_y][pos_x] = YELLOW;
		 	level1[pos_y-1][pos_x] = YELLOW;
	 	}
		else if(level1[pos_y+1][pos_x] == 0)
		{
	     fall();
		}
		/*
		if(level1[pos_y][pos_x+1] == EXIT_COLOR)
		{
			next_level();
		}
		*/
	}
	
	if(cur_level == 2)
	{
		//does not equal level length)
		if(pos_x != lvl_sizes[1] && level2[pos_y][pos_x+1] != TILE &&  level2[pos_y-1][pos_x+1] != TILE && level2[pos_y+1][pos_x] != 0)//cannot move through tile
		{
		 if(level2[pos_y][pos_x+1] == GREEN)
		 {
			 player_HP--;
		 }
		 if(player_HP == 0)
		 {
			 reset();
		 }
		 has_moved = 1;
		 level2[pos_y][pos_x] = 0;
		 level2[pos_y-1][pos_x] = 0;
		 pos_x++;
		 level2[pos_y][pos_x] = YELLOW;
		 level2[pos_y-1][pos_x] = YELLOW;
		}
		else if(level2[pos_y+1][pos_x] == 0)
		{
			fall();
		}
	}

}

void crouch()
{
	//moving_left = 0;
	//moving_right = 0;
	//in_air = 0;
	crouched = 1;
	if(in_air)//do nothing if an air
	{
		return;
	}
	
	has_moved = 1;
	
	if(cur_level == 1)
	{
			level1[pos_y-1][pos_x] = 0;
			level1[pos_y][pos_x] = YELLOW;	
	}
	
	if(cur_level == 2)
	{
			level2[pos_y-1][pos_x] = 0;
			level2[pos_y][pos_x] = YELLOW;
	}
}

void fall()
{
	//level 1
	if(cur_level == 1)
	{
		if(level1[pos_y+1][pos_x] != TILE && level1[pos_y+1][pos_x] != EXIT_COLOR)
		{
			has_moved = 1;
			level1[pos_y-1][pos_x] = 0;
			level1[pos_y][pos_x] = 0;
			pos_y++;
			level1[pos_y-1][pos_x] = YELLOW ;
		}
		else if(level1[pos_y+1][pos_x] == EXIT_COLOR)
		{
			next_level();
		}
		else
		{
			has_moved = 1;
			level1[pos_y][pos_x] = YELLOW;
			level1[pos_y-1][pos_x] = YELLOW;
			in_air = 0;
			crouched = 0;
		}
	}
	//level 2
	if(cur_level == 2)
	{
		if(level2[pos_y+1][pos_x] != TILE && level2[pos_y+1][pos_x] != BLUE)
		{
			has_moved = 1;
			level2[pos_y-1][pos_x] = 0;
			level2[pos_y][pos_x] = 0;
			//level1[pos_y+1][pos_x] = YELLOW;
			pos_y++;
			level2[pos_y-1][pos_x] = YELLOW ;
		}
		else if(level2[pos_y+1][pos_x] == BLUE)//restart section (die)
		{
			pos_y = 2;
			pos_x = 2;
			player_HP--;
			if(player_HP == 0)
			{
				cur_level = 1;
				level_size_index = 0;
				calc_render_range(lvl_sizes[level_size_index]);
				moving_right = 1;
				crouched = 0;
				in_air = 0;
				whipped = 0;
				pos_x = 2;
				pos_y = 6;
				player_HP = 4;
			}
		}
		else
		{
			has_moved = 1;
			level2[pos_y][pos_x] = YELLOW;
			level2[pos_y-1][pos_x] = YELLOW;
			in_air = 0;
			crouched = 0;
		}
	}

}
//potential bugs
//jumping out of bounds
//jumping through objects
void jump()
{
		has_moved = 1;
		in_air = 1;
		crouched = 1;
		///LEVEL 1
		if(cur_level == 1)
		{
			if(pos_y >= 4 && level1[pos_y-2][pos_x] == 0)
			{
				level1[pos_y][pos_x] = 0;
				level1[pos_y-1][pos_x] = 0;
				level1[pos_y-3][pos_x] = YELLOW;
				pos_y = pos_y-3;
			}
			else if (pos_y == 3 && level1[pos_y-2][pos_x] == 0) //only jump up 2 otherwise will end up out of range
			{
				level1[pos_y][pos_x] = 0;
				level1[pos_y-1][pos_x] = 0;
				level1[pos_y-3][pos_x] = YELLOW;
				pos_y -= 3;
			}
		}
		//LEVEL 2
		if(cur_level == 2)
		{
			if(pos_y >= 4 && level2[pos_y-2][pos_x] == 0)
			{
				level2[pos_y][pos_x] = 0;
				level2[pos_y-1][pos_x] = 0;
				level2[pos_y-3][pos_x] = YELLOW;
				pos_y = pos_y-3;
			}
			else if (pos_y == 3 && (level2[pos_y-2][pos_x] == EXIT_COLOR || level2[pos_y-3][pos_x] == EXIT_COLOR)) //only jump up 2 otherwise will end up out of range
			{
					next_level();
			}
			else if (pos_y == 3 && level1[pos_y-2][pos_x] == 0) //only jump up 2 otherwise will end up out of range
			{
				level2[pos_y][pos_x] = 0;
				level2[pos_y-1][pos_x] = 0;
				level2[pos_y-2][pos_x] = YELLOW;
				pos_y -= 2;
			}	
		}

}
void forward_jump()
{
//case control_rightB:
		in_air = 1;
		crouched = 1;
		//call jump first in state then get this one to move you forward
	 if(cur_level == 1)
	 {
		if(pos_x+1 != lvl_sizes[level_size_index] && moving_right && level1[pos_y][pos_x+1] != TILE && level1[pos_y][pos_x+1] != EXIT_COLOR)
		{
			level1[pos_y][pos_x] = 0;
			//level1[pos_y-1][pos_x] = 0;
			level1[pos_y][pos_x+1] = YELLOW;
			//	pos_y -= 3;
			pos_x++;
		}
		else if(pos_x != 0 && moving_left && level1[pos_y][pos_x-1] != TILE)
		{
		  level1[pos_y][pos_x] = 0;
		  //level1[pos_y-1][pos_x] = 0;
		  level1[pos_y][pos_x-1] = YELLOW;
		  pos_x--;
		}
	 }
	 //LEVEL 2 ///
	  if(cur_level == 2)
	  {
		  if(pos_x+1 != lvl_sizes[level_size_index] && moving_right && level2[pos_y][pos_x+1] != TILE && level2[pos_y][pos_x+1] != EXIT_COLOR)
		  {
			  level2[pos_y][pos_x] = 0;
			  //level2[pos_y-1][pos_x] = 0;
			  level2[pos_y][pos_x+1] = YELLOW;
			  //	pos_y -= 3;
			  pos_x++;
		  }
		  else if(pos_x != 0 && moving_left && level2[pos_y][pos_x-1] != TILE)
		  {
			  level2[pos_y][pos_x] = 0;
			  //level2[pos_y-1][pos_x] = 0;
			  level2[pos_y][pos_x-1] = YELLOW;
			  pos_x--;
		  }
	  }

}
void whip()//the destroyer of worlds CANNOT DESTROY TILES or END LEVEL
{
	whipped = 1;
	has_moved = 1;
	
	////////////// LEVEL 1 ////////////////////////////
	if(cur_level == 1)
	{
		//cannot whip when on right edge of map
		if(pos_x+2 < lvl_sizes[level_size_index] && moving_right)
		{
			//only times when 1 pixel tall
		   if((in_air || crouched))
		   {
			whip_loc = level1[pos_y][pos_x+1];
			whip_loc2 = level1[pos_y][pos_x+2];
		   }
		   else
		   {
			   //standing so shoot from head
			   whip_loc = level1[pos_y-1][pos_x+1];
			   whip_loc2 = level1[pos_y-1][pos_x+2];
		   }
		   
		  if((in_air || crouched) && whip_loc != TILE && whip_loc != EXIT_COLOR)
		  {
		     level1[pos_y][pos_x+1] = WHIP;
		        if(whip_loc2 != TILE && whip_loc2 != EXIT_COLOR)
				{
			     level1[pos_y][pos_x+2] = WHIP;
				}
		   }
		  //when standing
		  if((!in_air && !crouched) && whip_loc != TILE && whip_loc != EXIT_COLOR)
		  {
			level1[pos_y-1][pos_x+1] = WHIP;
			if(whip_loc2 != TILE && whip_loc2 != EXIT_COLOR)
			{
			  level1[pos_y-1][pos_x+2] = WHIP;
			}
		  }	
	    }
		
		///WHIP ON LEFT SIDE OF MAP
		//cannot whip when on right edge of map
		if(pos_x >= 2 && moving_left)
		{
			//only times when 1 pixel tall
			if((in_air || crouched))
			{
				whip_loc = level1[pos_y][pos_x-1];
				whip_loc2 = level1[pos_y][pos_x-2];
			}
			else
			{
				//standing so shoot from head
				whip_loc = level1[pos_y-1][pos_x-1];
				whip_loc2 = level1[pos_y-1][pos_x-2];
			}
			
			if((in_air || crouched) && whip_loc != TILE && whip_loc != EXIT_COLOR)
			{
				level1[pos_y][pos_x-1] = WHIP;
				if(whip_loc2 != TILE && whip_loc2 != EXIT_COLOR)
				{
					level1[pos_y][pos_x-2] = WHIP;
				}
			}
			//when standing
			if((!in_air && !crouched) && whip_loc != TILE && whip_loc != EXIT_COLOR)
			{
				level1[pos_y-1][pos_x-1] = WHIP;
				if(whip_loc2 != TILE && whip_loc2 != EXIT_COLOR)
				{
					level1[pos_y-1][pos_x-2] = WHIP;
				}
			}
		}
     }
	
	///////////////////////// LEVEL 2 ////////////////////////////
	if(cur_level == 2)
	{
		//cannot whip when on right edge of map
		if(pos_x+2 < lvl_sizes[level_size_index] && moving_right)
		{
			//only times when 1 pixel tall
			if((in_air || crouched))
			{
				whip_loc = level2[pos_y][pos_x+1];
				whip_loc2 = level2[pos_y][pos_x+2];
			}
			else
			{
				//standing so shoot from head
				whip_loc = level2[pos_y-1][pos_x+1];
				whip_loc2 = level2[pos_y-1][pos_x+2];
			}
			
			if((in_air || crouched) && whip_loc != TILE && whip_loc != EXIT_COLOR)
			{
				level2[pos_y][pos_x+1] = WHIP;
				if(whip_loc2 != TILE && whip_loc2 != EXIT_COLOR)
				{
					level2[pos_y][pos_x+2] = WHIP;
				}
			}
			//when standing
			if((!in_air && !crouched) && whip_loc != TILE && whip_loc != EXIT_COLOR)
			{
				level2[pos_y-1][pos_x+1] = WHIP;
				if(whip_loc2 != TILE && whip_loc2 != EXIT_COLOR)
				{
					level2[pos_y-1][pos_x+2] = WHIP;
				}
			}
		}
		
		///WHIP ON LEFT SIDE OF MAP
		//cannot whip when on right edge of map
		if(pos_x >= 2 && moving_left)
		{
			//only times when 1 pixel tall
			if((in_air || crouched))
			{
				whip_loc = level2[pos_y][pos_x-1];
				whip_loc2 = level2[pos_y][pos_x-2];
			}
			else
			{
				//standing so shoot from head
				whip_loc = level2[pos_y-1][pos_x-1];
				whip_loc2 = level2[pos_y-1][pos_x-2];
			}
			
			if((in_air || crouched) && whip_loc != TILE && whip_loc != EXIT_COLOR)
			{
				level2[pos_y][pos_x-1] = WHIP;
				if(whip_loc2 != TILE && whip_loc2 != EXIT_COLOR)
				{
					level2[pos_y][pos_x-2] = WHIP;
				}
			}
			//when standing
			if((!in_air && !crouched) && whip_loc != TILE && whip_loc != EXIT_COLOR)
			{
				level2[pos_y-1][pos_x-1] = WHIP;
				if(whip_loc2 != TILE && whip_loc2 != EXIT_COLOR)
				{
					level2[pos_y-1][pos_x-2] = WHIP;
				}
			}
		}
	}

}

//get rid of ANY whips on screen (does not matter direction
void retract_whip()
{
	//whipped = 0;
	for(unsigned char y = 0; y < 8 ; y++)
	{
		for(unsigned char x = start_render; x < end_render; x++)
		{
		    if(cur_level == 1)
			{
			  if(level1[y][x] == WHIP)
			  {
		        level1[y][x] = 0;
			  }	
			}
			if(cur_level == 2)
			{
				if(level2[y][x] == WHIP)
				{
					level2[y][x] = 0;
				}
			}
		 
		}
	}
}
void reset()
{
			cur_level = 1;
			level_size_index = 0;
			calc_render_range(lvl_sizes[level_size_index]);
			moving_right = 1;
			crouched = 0;
			in_air = 0;
			whipped = 0;
			pos_x = 2;
			pos_y = 6;
}


void next_level()
{
	if(cur_level == 1) //go to level 2
	{
		cur_level = 2; 
		//pos_x = 2; //set new position
		//pos_y = 6; //set new position y
				pos_x = 2;
				pos_y = 2;
		level_size_index = 1;//move to next level
	}
	else if(cur_level == 2)//go to lvl 3 top
	{
		//cur_level++;
		cur_level = 1;
		pos_x = 109;
		pos_y = 6; 
		//pos_x = 1;
		//pos_y = 2;
		level_size_index = 0;
	}
}