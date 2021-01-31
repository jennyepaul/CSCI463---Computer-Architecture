//***************************************************************************
//
//  hex.cpp
//  CSCI 463 Assignment 5
//
//  Created by Jennifer Paul (z1878099)
//
//***************************************************************************
#include "hex.h"
#include <iostream>
#include <stdint.h>
#include <string>
#include <sstream>
#include <iomanip>
/**
 * Prints & returns  an std::string with exactly 2 hex digits 
 * that represent 'i'
 *
 * @param i  uint8_t; will be printed as exactly 2 hex digits representing 
 * 	     8 bits of this value
 *
 * @return 8 bit string hex value of i
 *
 * @note the static_cast is necessary in this function and not others because
 * 	 it prevents the insertion operator from treating the 8-bit integer as 
 * 	 a char and printing it incorrectly
 ********************************************************************************/
std::string hex8(uint8_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
	return os.str();
}
/**
 * Prints and returns an std::string with 8 hex digits representing the 32 bits 
 * of 'i'
 *
 * @param i uint32_t; will be printed as exactly 8 hex digits 
 *
 * @return 32 bit string hex value of i
 *
 ********************************************************************************/
std::string hex32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << (i);
	return os.str();
}
/**
 * Prints and return an std::string with 8 hex digits, beginning with '0x',
 * represented by 'i' 
 *
 * @param i  uint32_t; will be printed as '0x' followed by 8 hex digits
 *
 * @return concatenation of '0x' followed by 32 bit string hex value of i
 ********************************************************************************/
 std::string hex0x32(uint32_t i)
{
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << (i);
	return std::string("0x")+hex32(i);
}

