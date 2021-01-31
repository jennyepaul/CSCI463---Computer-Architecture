//***************************************************************************
//
//  memory.cpp
//  CSCI 463 Assignment 5
//
//  Created by Jennifer Paul (z1878099)
//
//***************************************************************************
#include "memory.h"
#include "hex.h"
#include "rv32i.h"
#include <fstream>
#include <sstream>
#include <string>
#include <stdint.h>
#include <iomanip>
#include <iostream>
/**
 * Constuctor for the memory class that initializes data in the array to '0xa5'
 * and sets size equal to siz
 *
 * @param siz   size of memory, saved into the size variable
 *
 ********************************************************************************/
memory::memory(uint32_t siz)
{
	//save siz argument in the rounded up size variable
	siz = (siz+15) & 0xfffffff0;
	size = siz;

	//maybe make this uint32_t ???	
	mem = new uint8_t[size];

	//initialize the mem array with '0xa5'
	for(uint32_t i = 0; i < size; i++)
	{
		mem[i] = 0xa5;
	}
}
/**
 * Destructor for the memory class that frees the memory that was allocated
 * for the memory simulator
 *
 ********************************************************************************/
memory::~memory()
{
	delete [] mem;
	mem = NULL;
}
/**
 * Checks if the given address is in the memory simulator
 *
 * @param i    given address to be checked if present in memory simulator
 *
 * @return TRUE if the address is in the memory simulator, else output a 
 *         warning message and return FALSE
 *
 ********************************************************************************/
bool memory::check_address(uint32_t i) const
{
	//if address is in memory, return true
	if(i < size)
	
		return true;
	//else output a warning and return false
	else
	{
		std::cout << "WARNING: Address out of range: " << hex0x32(i) << std::endl;
		return false;
	}
	
}
/**
 * Returns the size (possibly rounded up) value
 *
 * @return Possible rounded up size vaule
 ********************************************************************************/
uint32_t memory::get_size()const
{
	return size;
}
/**
 * First checks if given address is in memory, then it will either return that
 * byte or return 0 to the caller.
 *
 * @param  addr    Address to be checked if present in memory simulator
 *
 * @return Address if present in the memory simulator, otherwise return 0
 *
 ********************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
	//check if address is valid, if TRUE then return the byte
	if(check_address(addr))
	{
		return mem[addr];
	}
	//ELSE it is not valid, return 0
	else
	{
		return 0;
	}
}
/**
 * Calls get8() function above twice, to get two bytes and then takes those
 * 2 bytes and combines in little endian order. 
 *
 * @param addr   The address that will have get8() called twice on & formatted
 *               into little endian
 *
 * @return The little endian order of the two bytes (little-endian)
 *
 ********************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
	//variable to hold little-endian version of addr
 	uint16_t little_endian;

	//variable to get the left-most bytes
	uint16_t left_most = get8(addr);

	//variable to get the right_most bytes
	uint16_t right_most= get8(addr + 1);

	//combine them together in little-endian
	little_endian = (right_most << 8 | left_most);

	return little_endian;
}
/**
 * Calls get16() function above twice and takes those bytes and combines
 * them in little endian order.
 *
 * @param addr   The address that will have get16() called twice on & formatted 
 *               into little endian.
 *
 * @return The little endian order of the four bytes (little-endian)
 ********************************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
	//variable to hold little-endian version of addr
	uint32_t little_endian;

	//variable to get the left_most bytes
	uint32_t left_most = get16(addr);

	//variable to get the right_most bytes
	uint32_t right_most = get16(addr + 2);

	//combine them together in little-endian
	little_endian = (right_most << 16| left_most);

	return little_endian;
}
/**
 * Calls check_address to make sure the address param is valid, if it is
 * then it will set mem[addr] to that val, if not it will return to caller
 *
 * @param addr  Address to be checked for validity
 * @param val   Value that will be stored at the address in memory, if address
 * 		is valid.
 ********************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
	//check if address is valid
	//if valid, then set byte in memory to that value
	if(check_address(addr))	
	{
		mem[addr] = val;
	}
}
/**
 * Calls set8() twice to store the given val in little-endian order into 
 * the simulated memory, starting at the address, given by the addr argument
 *
 * @param addr  Address where the value will be stored
 * @param val   Value to be stored in little-endian order at the address in
 * 		memory
 ********************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
	//variable to hold right_most byte 
	uint8_t right_most = (val & 0x00ff);

	//set byte at that address to right_most
	set8(addr, right_most);

	//variable to hold left_most byte
	uint8_t left_most = (val & 0xff00) >> 8;

	//set byte at that address to left_most
	set8(addr + 1, left_most);	
}
/**
 * Calls set16() twice to store the given val in little-endian order into
 * the simulated memory, starting at the address, given by the addr argument
 *
 * @param addr	 Address where the value will be stored
 * @param val	 Value to be stored in little-eendian order at the address in 
 * 		 memory.
 ********************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
	//variable to hold right_most byte
	uint16_t right_most = (val & 0x0000ffff);

	//set byte at that address to right_most
	set16(addr, right_most);

	//variable to hold left_most byte
	uint16_t left_most = (val & 0xffff0000) >> 16;		

	//set byte at that address to left_most
	set16(addr + 2, left_most);			
}
/**
 * Dumps entire contents of simulated memory array in hex with ASCII on 
 * the right.
 ********************************************************************************/
void memory::dump() const
{
	//declaration of ascii array
	char ascii[17];					
	ascii[16] = '\0';
	
	for(unsigned i = 0; i < size; i++)
	{
		//determines if we are to print an ASCII char or a dot for bytes that do not have
		//a valid printable value
		uint8_t ch = get8(i);
		ascii[i%16] = isprint(ch) ? ch : '.';

		//print the counter in hex
		if((i % 16) == 0)
		{
			std::cout << hex32(i) << ": ";
		}
		
		//print out the bytes, calling hex8 on them to get 2 hex digits at a time	
		std::cout << hex8(get8(i)) << " ";

		//print extra space to identify middle 
		if((i % 16) == 7)
		{
			std::cout << " ";
		}
		//ascii print out of file contents
		if((i % 16) == 15)
		{
			std::cout << "*" << ascii << "*" << std::endl;
		}		
	}
}
/**
 * Opens file (named fname) in binary mode, write contents of file
 * to the memory simulator (if it can open the file and it fits within
 * the given size)
 *
 * @param fname     Name of the file to be read
 *
 * @return  FALSE - if the file cannot be opened OR the program exceeds the 
 *          size
 *          TRUE - if the file can be opened, read, and written to mem array
 
 ********************************************************************************/
bool memory::load_file(const std::string &fname)
{      	
	std::ifstream infile(fname, std::ios::in|std::ios::binary);	 //open file in binary mode and read its contents into simulated memory
	uint32_t i = 0;

	//if the file can't open, output error message and return false
	if(!infile.is_open())						
	{	
		std::cout << std::endl;
		std::cerr << "Can’t open file '" << fname << "' for reading." << std::endl;
		return false;

	}
	//else it can open,so  check_address as we go, if valid then write to mem array (including white space) & increment counter
	//else, output error message, close file, and return false
	else
	{
		while(!infile.eof())
		{
			if(check_address(i))					
			{
				//put the file contents, including white space, into the memory array
				infile >> std::noskipws >> mem[i];
				i++;
			}
			else
			{
				std::cerr << "Program too big." << std::endl;
				infile.close();
				return false;
			}
		}
	}
	infile.close();
	return true;
}
