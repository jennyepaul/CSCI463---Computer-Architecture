//***************************************************************************
//
//  registerfile.cpp
//  CSCI 463 Assignment 5
//
//  Created by Jennifer Paul (z1878099)
//
//***************************************************************************
#include "rv32i.h"
#include "hex.h"
#include "registerfile.h"
#include <stdint.h>
#include <string>
#include <iostream>
#include <iomanip>

/**
 * Initializes the 32 registers in the RV32I simulator. Register 0 will always 
 * hold the value of 0, the rest of the registers will be initialized to the 
 * vlaue of f0f0f0f0
 *
 * @note    Register 0 will always hold the value of 0 and will never change
 * *****************************************************************************/
void registerfile::reset()
{
	//initialize 0x0000000 to register 0, since it always hold 0 value
	registers[0] = 0x00000000;

	//initialize all the other registers
	for(int32_t i = 1; i < 32; i++)
	{
		registers[i] = 0xf0f0f0f0;		//initialize all other registers
	}	
		
}
/**
 * This function is used to set the value of a register. If the register number
 * is NOT 0, that register will be set to the value. 
 *
 * @param r	The register number to set
 * @param val   Value that register will be set to
 *
 * @note   The register (r) is checked because register 0 will always hold 0*
 * *****************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
	
	if(r != 0)					//check if register 0 
	{
		registers[r] = val;			//assign register r to given val
	}
	
}
/**
 * This function is used to get the value in a register. If the register number
 * is NOT 0, that register will be get that  value. 
 *
 * @param r	The register number to get the contents of
 *
 * @note   The register (r) is checked because register 0 will always hold 0
 ******************************************************************************/
int32_t registerfile::get(uint32_t r) const
{	
	if(r != 0)
	{
		return registers[r];			//return value in register
	}
	else
	{
		return 0;
	}
}
/**
 * This function is used to dump the contents of the regiseters. It will
 * print the registers in 4 rows, with 8 registers in each row. Each row is 
 * labeled with the register that starts that row.  This function also makes 
 * use of the std::right to right-justify the output.
 ******************************************************************************/
void registerfile::dump() const
{
	//print out registers 0 - 7
	std::cout << std::right << std::setw(3)  << "x0";
	for(int32_t i = 0; i < 8; i++)
	{
		std::cout << " " << hex32(registers[i]);	//print the contents of the register out
	}
        std::cout << std::endl;

	//print out registers 8 - 15
	std::cout << std::right << std::setw(3) << "x8";
	for(int32_t i = 8; i < 16; i++)
	{
		std::cout << " " << hex32(registers[i]);	//print the contents of the register out
	}
        std::cout << std::endl;

	//print out registers 16-23
	std::cout << std::right <<  "x16";
	for(int32_t i = 16; i < 24; i++)
	{
		std::cout << " " << hex32(registers[i]);	//print the contents of the register out
	}
        std::cout << std::endl;

	//print out registers 24-31
	std::cout << std::right <<  "x24";
	for(int32_t i = 24; i < 32; i++)
	{
		std::cout << " " << hex32(registers[i]);	//print the contents of the register out
	}	
}
