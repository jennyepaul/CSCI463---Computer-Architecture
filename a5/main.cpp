//***************************************************************************
//
//  main.cpp
//  CSCI 463 Assignment 5
//
//  Created by Jennifer Paul (z1878099)
//
//***************************************************************************
#include "memory.h"
#include "rv32i.h"
#include "registerfile.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
/**
* Print a usage message and abort the program .
*********************************************************************/
static void usage()
{
	std::cerr << "Usage: rv32i [-m hex-mem-size] [-d] [-i] [-r] [-z] [-l execution-limit] infile " << std::endl;
	std::cerr << " 	  -m specify memory size (default = 0x10000)" << std::endl;
	std::cerr << "          -l specify limit amounr (default = 0)" << std::endl;
 	exit(1);
}

/**
* Read a file of RV32I instructions and execute them .
* @param argc Argument counter
* @param argv Argument values that the user passes
*             
* @return Returns 0 upon successful completion of the various functions
*
********************************************************************************/

int main (int argc, char **argv )
{
	uint32_t memory_limit = 0x10000; 	// default memory size = 64 k
	uint64_t limit = 0;			//default limit to run forever
 	int opt;

	//the following are bool variables to see what options the user has entered
	bool disasm = false;
	bool instruct = false;
	bool isMemory = false;
	bool predump = false;
	bool postdump = false;

	while ((opt = getopt(argc, argv, "dirzm:l:")) != -1)
	{
		//switch to see what options the user has entered, if they entered any, the corresponding
		//bool variable is set to 'true'
 		switch (opt)
 		{
 		case 'm':
 			memory_limit = std::stoul(optarg, nullptr, 16);
			isMemory = true;
 			break;
		case 'd':
			disasm = true;
			break;
		case 'i':
			instruct = true;
			break;	
		case 'l':
		        limit = atoi(optarg);
			break;
		case 'r':
			predump = true;
			break;
		case 'z':
			postdump = true;
			break;
 		default: /* ’? ’*/ 
			usage();		
		}
 	}

 	if (optind >= argc )
 		usage(); 			// missing filename

	//if they forgot to input memory, output usage
	if(isMemory == false)
	{
		usage();
	}

	memory mem(memory_limit);
	

        if (!mem.load_file(argv[optind]))
 		usage();

	rv32i sim(&mem);
	sim.reset();

	//if they entered 'd' option, disassemble then reset
	if(disasm == true)
	{
		sim.disasm();
		sim.reset();
	}
	//if they entered 'i' option, then show instructions 
	if(instruct == true)
	{
		//if they entered 'r' option then print registers dump before instructions 
		if(predump == true)
		{
			sim.set_show_registers(true);
		}
		sim.set_show_instructions(true);
	}	
	//run the simulator
	sim.run(limit);	

	//if they entered 'z' option, print final memory and register dump 
        if(postdump == true)
	{
		sim.dump();
		mem.dump();	//memory dump
	}
	
 	return 0;
}
