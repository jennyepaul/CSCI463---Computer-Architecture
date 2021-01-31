
//***************************************************************************
//
//  registerfile.h
//  CSCI 463 Assignment 5
//
//  Created by Jennifer Paul (z1878099)
//
//***************************************************************************
#ifndef registerfile_h
#define registerfile_h
#include "rv32i.h"
#include <stdint.h>
#include "memory.h"
#include <string>
#include "hex.h"
#include "stdint.h"
#include <iostream>
#include <string>
#include <sstream>
class registerfile
{
	public:
		void reset();
		void set(uint32_t r, int32_t val);
		int32_t get(uint32_t r) const;
		void dump() const;
	private:
		int32_t registers[32];				
	
};
#endif
