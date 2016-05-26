#ifndef GAME_OBJECTS_H
#define GAME_OBJECTS_H
//AUTHOR: Jonathan Padilla jpadi004@ucr.edu

//defines whether or not belmont is in the air after a jump,
//cannot redirect which direction to fall
unsigned char in_air;

unsigned char has_moved;//used to determine when to calculate next thing to render
//main player character Simon Belmont
typedef struct Simon {
unsigned char HP;
char* side_weapon;
unsigned char whip_lvl;
} Simon;

typedef struct enemy {
unsigned char HP;
const char* color;
} enemy;

typedef struct bat_boss {
unsigned char HP;
const char* color;
}bat_boss;

//0x00FFFF
//first two bits control blue lines
//0xFF00FF
//middle bits control red lines
//0xFFFF00
//right bits control green lines

//LIGHT BLUE
//unsigned long square_row[] = {0xFFFFFF, 0xFFFFFF, 0xFFFFFFF,0xFFFF7F, 0xFFFFFF,0x7FFFFF};
//unsigned char square_col[] = {0x00, 0x01, 0x01, 0x01, 0x00, 0x01};
//PINK
//unsigned long square_row[] = {0xFFFFFF, 0xFF7FFF, 0xFF7FFF,0xFF7FFF, 0xFFFFFF,0x7FFFFF};
//unsigned char square_col[] = {0x00, 0x01, 0x01, 0x01, 0x00, 0x01};
#endif //GAME_OBJECTS