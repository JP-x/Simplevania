
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef BIT_H
#define BIT_H

////////////////////////////////////////////////////////////////////////////////
//Functionality - Sets bit on a PORTx
//Parameter: Takes in a uChar for a PORTx, the pin number and the binary value 
//Returns: The new value of the PORTx
unsigned char SetBit(unsigned char pin, unsigned char number, unsigned char bin_value) 
{
	return (bin_value ? pin | (0x01 << number) : pin & ~(0x01 << number));
}

////////////////////////////////////////////////////////////////////////////////
//Functionality - Sets bit on a PORTx
//Parameter: Takes in a uChar for a PORTx, the pin number and the binary value
//Returns: The new value of the PORTx
unsigned long SetLongBit(unsigned long long_var, unsigned char bit_to_change, unsigned char bin_value)
{
	return (bin_value ? long_var | (0x000001 << bit_to_change) : long_var & ~(0x000001 << bit_to_change));
}

unsigned long SetLongREDBit(unsigned long long_var, unsigned char bit_to_change, unsigned char bin_value)
{
	return (bin_value ? long_var | (0x000100 << bit_to_change) : long_var & ~(0x000100 << bit_to_change));
}


unsigned long SetLongGREENBit(unsigned long long_var, unsigned char bit_to_change, unsigned char bin_value)
{
	return (bin_value ? long_var | (0x000001 << bit_to_change) : long_var & ~(0x000001 << bit_to_change));
}

unsigned long SetLongBLUEBit(unsigned long long_var, unsigned char bit_to_change, unsigned char bin_value)
{
	return (bin_value ? long_var | (0x010000 << bit_to_change) : long_var & ~(0x010000 << bit_to_change));
}


////////////////////////////////////////////////////////////////////////////////
//Functionality - Gets bit from a PINx
//Parameter: Takes in a uChar for a PINx and the pin number
//Returns: The value of the PINx
unsigned char GetBit(unsigned char port, unsigned char number) 
{
	return ( port & (0x01 << number) );
}

#endif //BIT_H