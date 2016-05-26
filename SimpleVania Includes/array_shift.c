/*
Array Shifter.c
Used to shift an array over left right up and down

Used in render state machines
*/

//used for shifting colored columns specifically

void shift_left(unsigned char size_array, unsigned long* l_arr)
{
	//left most bound on LCD matrix 
	//long 0x FF FF FF
	//0x7F
	//have to check each color
	//1000 0000 &
	//0111 1111
	//if positive then that bit is not at the edge
	if(0x800000 & l_arr[0]) || 0x008000 & l_arr[0] || 0x000080 & l_arr[0]0 )
	{
		for(unsigned char z = 0; z <size_array ; z++)
		{
			//if(square_col[z] != 0x7F)
			//{
			square_col[z] = (square_col[z] << 1) + 0x000001;
			//}
		}
}

void shift_up(unsigned char size_array, unsigned long*)
{
	
}

void shift_down(unsigned char size_array, unsigned long*)
{
	
}

void shift_right(unsigned char size_array, unsigned long*)
{
	
}


//SHORT DATA ARRAYS
void shift_left(unsigned char size_array, unsigned short*)
{
	
}

void shift_up(unsigned char size_array, unsigned short*)
{
	
}

void shift_down(unsigned char size_array, unsigned short*)
{
	
}

void shift_right(unsigned char size_array, unsigned short*)
{
	
}

