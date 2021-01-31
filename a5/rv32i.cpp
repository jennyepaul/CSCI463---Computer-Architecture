
//***************************************************************************
//
//  rv32i.cpp
//  CSCI 463 Assignment 5
//
//  Created by Jennifer Paul (z1878099)
//
//***************************************************************************
#include "memory.h"
#include "hex.h"
#include "rv32i.h"
#include "stdint.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cassert>
#include <iomanip>
/**
 * Saves the m value in the mem variable
 *
 * @param m	Memory variable that will get stored in mem.
 ********************************************************************************/
rv32i::rv32i(memory *m)
{
	//saving m in mem variable
	 mem = m;
}
/**
 * Disassembles the instructions in memory.
 *
 * First, two variables are declared to store the memory size and instruction in.
 * Next, get_size() function from the memory class is used to get the size of the
 * simulated memory, divided by 4, and then used in the for loop to go through
 * and dissemble the instruction, and the pc is set initially to 0. The for loop
 * then goes through and prints the address in memory in hex, fetches, and prints
 * the instruction in hex, passes that instruction to the decode function, 
 * outputs that decoded instruction, and increments the pc by 4. 
 ********************************************************************************/
void rv32i::disasm(void)
{
	uint32_t instruction;
	uint32_t memory_siz;

	//get the total size of the memory
 	uint32_t total_siz = mem->get_size();
	//divide by 4 since each instruction is 4 bytes
	memory_siz = total_siz / 4;
	std::string decoded_string;
	pc = 0;

	//go through and dissamble the instructions in the simulated memory
	for(unsigned i = 0; i < memory_siz; i++)
	{
		std::cout << hex32(pc) << ": ";					//printing 32-bit address at pc in hex
		instruction = mem->get32(pc);					//fetching 32-bit instruction at address in pc
		std::cout << hex32(instruction) << "  ";			//print the instruction as 32-bit
		decoded_string = decode(instruction);				//pass instruction to decode to decode it
		std::cout << decoded_string;					//output the returned string
		pc += 4;							//increment to next address
		std::cout << std::endl;
	}
	
}
/**
 * Uses masking to get the opcode from the insn value. 
 *
 * @param insn	Instruction to get opcode from
 *
 * @return Extracted opcode.
 ********************************************************************************/
uint32_t rv32i::get_opcode(uint32_t insn)
{
	return (insn & 0x0000007f);				//extract the opcode from the insn
}
/**
 * Uses masking to get the rd value  from the insn value. 
 *
 * @param insn	Instruction to get rd  from
 *
 * @return Extracted rd value.
 ********************************************************************************/
uint32_t rv32i::get_rd(uint32_t insn)
{
	return (insn & 0x00000f80) >> 7;			//extract the destination register
}
/**
 * Uses masking to get the funct3 from the insn value. 
 *
 * @param insn	Instruction to get funct3 from
 *
 * @return Extracted funct3 value
 ********************************************************************************/
uint32_t rv32i::get_funct3(uint32_t insn)
{
	return (insn & 0x00007000) >> 12;			//extract the funct3 value
}
/**
 * Uses masking to get the rs1  from the insn value. 
 *
 * @param insn	Instruction to get rs1 from
 *
 * @return Extracted rs1 value.
 ********************************************************************************/
uint32_t rv32i::get_rs1(uint32_t insn)
{
	return (insn & 0x000f8000) >> 15;			//extract the rs1 value
}

/**
 * Uses masking to get the rs2 from the insn value. 
 *
 * @param insn	Instruction to get rs2  from
 *
 * @return Extracted rs2 value.
 ********************************************************************************/
uint32_t rv32i::get_rs2(uint32_t insn)
{
	return (insn & 0x01f00000) >> 20;			//extract the rs2 value
}

/**
 * Uses masking to get the funct7 from the insn value. 
 *
 * @param insn	Instruction to get funct7 from
 *
 * @return Extracted funct7 value.
 ********************************************************************************/
uint32_t rv32i::get_funct7(uint32_t insn)
{
	return (insn & 0xfe000000) >> 25;			//extract the funct7 
}

/**
 * Uses masking to get the funct7 value for ebreak and ecall
 * from the insn value, since that funct7 value is 12 bits long and not 7 like other
 * funct7 values. 
 *
 * @param insn	Instruction to get ebreak and ecall funct7 value from
 *
 * @return Extracted funct7 value.
 ********************************************************************************/
uint32_t rv32i::get_special_funct7(uint32_t insn)
{
	return (insn & 0xfff00000) >> 20;			//extract the funct7 value for ebreak and ecall
}

/**
 * Uses masking to get the fence funct7 from the insn value, since the fence
 * funct7 value is 4 bits long and not 7 like other funct7 values.
 *
 * @param insn	Instruction to get fence funct7 from
 *
 * @return Extracted fence funct7 value.
 ********************************************************************************/
uint32_t rv32i::get_fence_funct7(uint32_t insn)
{
	return (insn & 0xf0000000);				//extract fence funct7 value
}
/**
 * Uses masking to get the succ value from the insn for the fence instruction.
 *
 * @param insn	Instruction to get succ from
 *
 * @return Extracted succ value.
 ********************************************************************************/
uint32_t rv32i::get_succ(uint32_t insn)
{
	return (insn & 0x00f00000) >> 20;			//extract the succ value for fence instruction
}
/**
 * Uses masking to get the pred  value  from the insn for the fence instruction.
 *
 * @param insn	Instruction to get pred from
 *
 * @return Extracted pred value.
 ********************************************************************************/
uint32_t rv32i::get_pred(uint32_t insn)
{
	return (insn & 0x0f000000) >> 24;			//extract the pred value for fence instruction
	
}
/**
 * Uses masking and bitwise operators to get the imm_i value from the insn
 *
 * @param insn	Instruction to get imm_i value from
 *
 * @return Extracted imm_i value
 ********************************************************************************/
int32_t rv32i::get_imm_i(uint32_t insn)
{
	int32_t imm_i = (insn & 0xfff00000) >> (20 - 0);	//extract & shift bits

	if(insn & 0x80000000)					//sign extend
		imm_i |= 0xfffff000;

	return imm_i;
}
/**
 * Uses masking and bitwise operators to get the imm_u value from the insn
 *
 * @param insn	Instruction to get imm_u value from
 *
 * @return Extracted imm_u value
 ********************************************************************************/
int32_t rv32i::get_imm_u(uint32_t insn)
{
	int32_t imm_u = (insn & 0xfffff000);			//mask bits 0-11 with 0's

	return imm_u;
}
/**
 * Uses masking and bitwise operators to get the imm_b value from the insn
 *
 * @param insn	Instruction to get imm_b from
 *
 * @return Extracted imm_b value
 ********************************************************************************/
int32_t rv32i::get_imm_b(uint32_t insn)
{
	//first 3 lines set first bit to 0
	int32_t imm_b = ((insn << 31) & 0x00000000);		
	imm_b >>= 31;						
	imm_b |= (insn & 0x00000001) & 0;
	imm_b |= (insn & 0x7e000000) >> (25 - 5);		//extract & shift bits
	imm_b |= (insn & 0x80000000) >> (31-12);
	imm_b |= (insn & 0x00000f00) >> (8-1);			//extract & shift bits
	imm_b |= (insn & 0x00000080) << (11-7);			//move bit 7 to bit 11 position

	if(insn & 0x80000000)					//sign-extend
		imm_b |= 0xffffe000;

	return imm_b;
}
/**
 * Uses masking and bitwise operators to get the imm_s value from the insn
 *
 * @param insn	Instruction to get imm_s from
 *
 * @return Extracted imm_s value
 ********************************************************************************/
int32_t rv32i::get_imm_s(uint32_t insn)
{
	int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);	//extract & shift bits 5-11
	imm_s |= (insn & 0x00000f80) >> (7-0);			//extract and shift bits 0-4

	if(insn & 0x80000000)					//sign-extend
		imm_s |= 0xfffff000;
	
	return imm_s;
}
/**
 * Uses masking and bitwise operators to get the imm_j value from the insn
 *
 * @param insn	Instruction to get imm_j from
 *
 * @return Extracted imm_j value
 ********************************************************************************/
int32_t rv32i::get_imm_j(uint32_t insn)
{
	int32_t imm_j = ((insn << 31) & 0x00000000);		//get a 0 on the first bit
	imm_j |= (insn & 0x000ff000);				//bits 12-19 stay in same spot
	imm_j |= (insn & 0x00100000) >> (20-11);		//shift bit 20 to bit 11 position
	imm_j |= (insn & 0x7fe00000) >> (21-1);			//get bits 21-30 and shift left
	imm_j |= (insn & 0x80000000) >> (31-20);		//extract and shift bit 31 to bit position 20
	
	if(insn & 0x80000000)					//sign extend 11 bits
		imm_j |= 0xffe00000;

	return imm_j;
}
/**
 * Creates the string of text to be returned for the default case in the
 * switch statement in the decode() function 
 *
 * @return error_insn variable that contains error string message
 * *****************************************************************************/
std::string rv32i::render_illegal_insn() const
{
	std::string error_insn = "ERROR: UNIMPLEMENTED INSTRUCTION";
	return error_insn;
	
}
/**
 * Creates the string of text to be returned in decode for the lui
 * instruction.
 *
 * This function also makes use of get_rd and get_imm_u functions
 *
 * @param insn	Instruction to decode and get rd and imm_u value from  
 *
 * @return String text of the instruction name, lui , followed by the rd and
 * imm_u value
 * *****************************************************************************/
std::string rv32i::render_lui(uint32_t insn) const
{
	//following 2 lines obtain rd and imm_u needed for lui instruction
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	//this returns the lui  instruction name, followed by destination register, and the imm_u value
	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "lui"
		<< "x" << std::dec << rd << ",0x" << std::hex << ((imm_u  >> 12) & 0x0fffff);
       	return os.str();

}
/**
 * Creates the string of text to be returned in decode for the auipc 
 * instruction.
 *
 * This function also makes use of get_rd and get_imm_u functions
 *
 * @param insn	Instruction to decode and get rd and imm_u value from  
 *
 * @return String text of the instruction name, auipc , followed by the rd and
 * imm_u value
 * *****************************************************************************/
std::string rv32i::render_auipc(uint32_t insn) const
{
	//the following 2 lines obtain the rd and imm_u values needed for auipc
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	//this returns the auipc instruction name, followed by destination register, and the imm_u value
	//that is extracted using a function from above
	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "auipc "
		<< "x" << std::dec << rd << ",0x" << std::hex << ((imm_u >> 12) & 0x0fffff);
	return os.str();

	
}
/**
 * Creates the string of text to be returned in decode for the rtype 
 * instructions
 *
 * This function also makes use of get_rd, get_rs1, and get_rs2 functions to 
 * obtain the correct values from the insn value.
 *
 * @param insn	Instruction to decode and get rs1, rs2, and rd values from
 * @param mnemonic Variable that will be set to the name of the instrcution in 
 * the decode function 
 *
 * @return String text of the instruction mnemonic, followed by the rd, rs1, and
 * rs2 values
 * *****************************************************************************/
std::string rv32i::render_rtype(uint32_t insn, const char *mnemonic) const
{
	//the following 3 lines obtain the needed fields for the R type instructions
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);

	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
		<< "x" << std::dec << rd << ",x" << std::dec << rs1 << ",x" << std::dec << rs2;
	return os.str();
	
}
/**
 * Creates the string of text to be returned in decode for the load itype 
 * instruction
 *
 * This function also makes use of get_rd, get_rs1, and get_imm_i functions to 
 * obtain the correct values from the insn value.
 *
 * @param insn	Instruction to decode and get rs1, rs2, and imm_i values from.*
 * @param mnemonic Variable that will be set to the name of the instrcution in 
 * the decode function 
 *
 * @return String text of the instruction mnemonic, followed by the rd value, 
 * imm_i value, and then rs1 value
 * *****************************************************************************/
std::string rv32i::render_itype_load(uint32_t insn, const char *mnemonic) const
{
	//the following 3 lines obtain the needed fields for the I type load instructions
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
		<< "x" << std::dec << rd << "," << std::dec << (imm_i) 
		<< "(x" << std::dec << rs1 << ")";
	return os.str();

}
/**
 * Creates the string of text to be returned in decode  for the alu itype 
 * instruction
 *
 * This function also makes use of get_rd, get_rs1, and get_imm_i functions to 
 * obtain the correct values from the insn value.
 *
 * @param insn	Instruction to decode and get rs1, rs2, and imm_i values from.*
 * @param mnemonic Variable that will be set to the name of the instrcution in 
 * the decode function 
 *
 * @return String text of the instruction mnemonic, followed by the rd value, 
 * rs1 value and the imm_i
 * *****************************************************************************/
std::string rv32i::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const
{
	//the following 3 lines obtain the needed fields for the I type ALU instructions
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	imm_i = get_imm_i(insn);

	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
		<< "x" << std::dec << rd << ",x" << std::dec << rs1 << "," << std::dec << imm_i; 
	return os.str();
}
/**
 * Creates the string of text to be returned in decode  for the itype 
 * instruction that uses shamt_i
 *
 * This function also makes use of get_rd, get_rs1, and get_imm_i functions to 
 * obtain the correct values from the insn value. A shamt_i variable is created
 * to store the shift immediate value which is obtained by zero-ing out 27 bits
 * and then using bitwise operators to extract bits 0-4.
 *
 * @param insn	Instruction to decode and get rd, rs1, and imm_i values from. 
 * @param mnemonic Variable that will be set to the name of the instrcution in 
 * the decode function 
 *
 * @return String text of the instruction mnemonic, followed by the rd value, 
 * rs1 value and the shamt_i value.
 * *****************************************************************************/
std::string rv32i::render_itype_alu_shamt(uint32_t insn, const char *mnemonic, int32_t imm_i) const
{
	//the following 6 lines obtain the needed fields for the alu shift immediate I type
	//instructions, and make use of shifting and masking to extract the shamt_i value
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	imm_i = get_imm_i(insn);
	
	uint32_t shamt_i = (imm_i << 27);	//zero out 27 bits for the zero extension
	shamt_i >>= 27;				//shift back
	shamt_i = shamt_i & 0xfffffe1f;


	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
		<< "x" << std::dec << rd << ",x" << std::dec << rs1 << "," <<  std::dec << shamt_i; 
	return os.str();
}

/**
 * Creates the btype instruction string of text that will be returned in decode
 * function
 *
 * This function also makes use of get_rs1, get_rs2, and get_imm_b functions to 
 * obtain the correct values from the insn value. The imm_b value is used to 
 * calculate the pcrel_13 value by adding the pc value to the imm_b value.
 *
 * @param insn	Instruction to decode and get rs1, rs2, and imm_b values from.
 * @param mnemonic Variable that will be set to the name of the instrcution in 
 * the decode function 
 *
 * @return String text of the instruction mnemonic, followed by the rs1 value, 
 * rs2 value, and finally the pcrel_13 value
 * *****************************************************************************/
std::string rv32i::render_btype(uint32_t insn, const char *mnemonic) const
{
	//the following 4 lines obtain the needed fields for the B type instruction
	//and calculate the pcrel_13 amount 
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
 	int32_t imm_b = get_imm_b(insn);
	int32_t pcrel_13 = imm_b + pc;


	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
		<< "x" << std::dec << rs1 << ",x" << std::dec << rs2 << ",0x" << std::hex << pcrel_13;
	return os.str();

}
/**
 * Creates the stype instruction string of text that will be returned in decode
 * function
 *
 * This function also makes use of get_rs1 , get_rs2 and get_imm_s functions to 
 * obtain the correct values from the insn value.
 *
 * @param insn	Instruction to decode and get rs1, rs2, and imm_s value from 
 *
 * @param mnemonic Variable that will be set to the name of the instrcution in 
 * the decode function
 *
 * @return String text of the instruction mnemonic, followed by the decimal rs2
 * value, imm_s value, then decimal rs1 value
 * *****************************************************************************/
std::string rv32i::render_stype(uint32_t insn, const char *mnemonic) const
{
	//the following 3 lines obtain the needed fields for the S type instructions
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_s = get_imm_s(insn);

	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << mnemonic
		<< "x" << std::dec << rs2 << "," << std::dec << imm_s  << "(x" << std::dec << rs1 << ")"; 
	return os.str();
}
/**
 * Creates the jal instruction string of text that will be returned in decode
 * function
 *
 * This function also makes use of get_rd and get_imm_j functions to obtain the 
 * correct values from the insn value. After the imm_j value is extracted, it 
 * will add the pc value to it to get the pcrel_21 value that will be the final
 * value in the string of text
 *
 * @param insn	Instruction to decode and get rd, and imm_j values from.
 * @param mnemonic Variable that will be set to the name of the instrcution in 
 * the decode function 
 *
 * @return String text of the instruction name, 'jal', followed by the rd value,
 * the the pcrel_21 value.
 * *****************************************************************************/
std::string rv32i::render_jal(uint32_t insn) const
{ 
	//the following 3 lines obtain the needed fields to print out the jal instruction
	uint32_t rd = get_rd(insn);
	int32_t imm_j = get_imm_j(insn);
	int32_t pcrel_21 = imm_j + pc;

	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "jal "
		<< "x" << std::dec << rd << ",0x" << std::hex << pcrel_21;

	return os.str();

}
/**
 * Creates the jalr instruction string of text that will be returned in decode
 * function
 *
 * This function also makes use of get_rd, get_rs1, and get_imm_i functions to
 * obtain the correct values from the insn value.
 *
 * @param insn	Instruction to decode and get rd, rs1, and imm_i value from.
 *
 * @return String text of the instruction name, 'jalr', followed by the rd value,
 * the imm_i value, and finally the rs1 value in parentheses.
 ********************************************************************************/
std::string rv32i::render_jalr(uint32_t insn) const
{
	// the following 3 lines obtain the needed fields to print out the jalr instruction
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);
	
	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "jalr "
		<< "x" << std::dec << rd << "," <<  std::dec << (imm_i) << "(x" << std::dec << rs1 << ")";
       	return os.str();
}

/**
 * Creates the ecall instruction string of text that will be returned in decode 
 * function.
 *
 * @return String text of the instruction name, 'ecall'
 ********************************************************************************/
std::string rv32i::render_ecall( uint32_t insn) const
{
	std::ostringstream os;
	os << std::left << "ecall";
       	return os.str();

}
/**
 * Creates the ebreak instruction string of text that will be returned in decode
 * function.
 *
 * @return String text of the instruction name, 'ebreak'
 ********************************************************************************/
std::string rv32i::render_ebreak(uint32_t insn) const
{
	std::ostringstream os;
	os << std::left << "ebreak";
       	return os.str();

}
/**
 * Creates the fence instruction string of text that names the instruction
 * and then prints the pred and succ values next to it. 
 *
 * This function declares 4 variables to correspond to the values of one, two,
 * four, and eight. It then declares the two strings, 'pred' and 'succ' which will
 * be outputted, and then uses the get_pred and get_succ functions to optain the 
 * value of the predessor and successor. Under that there are two switch statements, 
 * depending on the value of the predeccesor and successor obtained it will create a
 * string using the 4 variables (one, two, four, eight)  and concatenate the 
 * variables if needed. 
 *
 * @param insn	Instruction obtained from the disasm() function
 *
 * @return The string text of the instruction name, 'fence' and then its pred and
 * 	   successor values.
 ********************************************************************************/
std::string rv32i::render_fence(uint32_t insn) const
{
	
	std::string eight = "i";			//i value is 8, so create a variable to store that
	std::string four = "o";				// o value is 4, so create a variable to store that
	std::string two = "r";				//r value is 2, so create a variable to store that
	std::string one = "w";				//w value is 1, so create a variable to store that
	
	//following 2 strings are what will get returned/printed with the fence instruction
	std::string succ;
	std::string pred;

	//use pred_val and succ_val function to get the value for pred and succ
	uint32_t pred_val = get_pred(insn);
	uint32_t succ_val = get_succ(insn);
	
	// the following switch statement will go through and based on the calculated pred_val it will set the string 
	// pred = to the correct corresponding value, if the value is not 1, 2, 4, or 8, it will concatenate the correct
	// values together
	switch(pred_val)
	{
	case One:
	
		pred = one;
		break;
	case Two:
		pred = two;
		break;
	case Three:
		pred = two + one;
		break;
	case Four:
		pred = four;
		break;
	case Five:
		pred = four + one;
		break;
	case Six:
		pred = four + two;
		break;
	case Seven:
		pred = four + two + one;
		break;
	case Eight:
		pred = eight;
		break;
	case Nine:
		pred = eight + one;
		break;
	case Ten:
		pred = eight + two;
		break;
	case Eleven:
		pred = eight + two + one;
		break;
	case Twelve:
		pred = eight + four;
		break;
	case Thirteen:
		pred = eight + four + one;
		break;
	case Fourteen:
		pred = eight + four + two;
		break;
	case Fifteen:
		pred = eight + four + two + one;
		break;
	}
	
	// the following switch statement will go through and based on the calculated succ_val it will set the string 
	// succ = to the correct corresponding value, if the value is not 1, 2, 4, or 8, it will concatenate the correct
	// values together
	switch(succ_val)
	{
	case One:
	
		succ = one;
		break;
	
	case Two:
		succ = two;
		break;
	case Three:
		succ = two + one;
		break;
	case Four:
		succ = four;
		break;
	case Five:
		succ = four + one;
		break;
	case Six:
		succ = four + two;
		break;
	case Seven:
		succ = four + two + one;
		break;
	case Eight:
		succ = eight;
		break;
	case Nine:
		succ = eight + one;
		break;
	case Ten:
		succ = eight + two;
		break;
	case Eleven:
		succ = eight + two + one;
		break;
	case Twelve:
		succ = eight + four;
		break;
	case Thirteen:
		succ = eight + four + one;
		break;
	case Fourteen:
		succ = eight + four + two;
		break;
	case Fifteen:
		succ = eight + four + two + one;
		break;
	}

	std::ostringstream os;
	os << std::setw(mnemonic_width) << std::setfill(' ') << std::left << "fence " <<  pred << "," << succ;
       	return os.str();
}
/**
 * Executes and renders the illegal message if halt is true and pos
 * is not nullptr.
 *
 * This function also makes use of the render_illegal_insn() function
 *
 * @param insn  Instruction obtained 
 * @param pos	Output stream object used to print execution
 *
 * *****************************************************************************/
void rv32i::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
	halt = true;						//set halt flag
	if(pos != nullptr)
	{
		render_illegal_insn();				//output illegal message
	}

}
/**
 * Executes and renders the lui instruction
 *
 * This function also makes use of get_rd and get_imm_u functions, as well as 
 * the render_lui function to output the lui name, and the rd,imm_u values.
 * After outputting the render and the output stream object containing the 
 * execution, it sets the rd to the imm_u value, and increments the pc + 4 
 *
 * @param insn	Instruction to decode and get rd and imm_u value from  
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_lui(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);
	
	if(pos)
	{
		//set 's' string to rendered lui instruction
		std::string s = render_lui(insn);		//call to render_lui function
		s.resize(instruction_width, ' ');

		//output the rendered string and execution of lui
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(imm_u);
	}

	//set the destination register to imm_u and increment pc+4
	regs.set(rd, imm_u);
	pc+=4;

}
/**
 * Executes and renders the auipc instruction
 *
 * This function also makes use of get_rd and get_imm_u functions, as well as 
 * the render_auipc function to output the auipc name, and the rd,imm_u values.
 * Also, the value to be stored into teh rd is the addition of pc to imm_u. After
 * outputting the render, and output stream, the rd is set to the value calculated
 * and the pc is incremented + 4.
 *
 * @param insn	Instruction to decode and get rd and imm_u value from  
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_auipc(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn);

	int32_t val = (imm_u + pc);			//add the value of pc  + imm_u value
	
	if(pos)
	{
		//set 's' string to rendered auipc instruction
		std::string s = render_auipc(insn);	//call to render_auipc function
		s.resize(instruction_width, ' ');

		//output the destination register, the pc, imm_u, and the value of adding imm_u + pc
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(pc) << " + " <<  hex0x32(imm_u)
			<< " = " << hex0x32(val);
	}

	//set destination register to the value and increment pc
	regs.set(rd, val);
	pc+=4;

}
/**
 * Executes and renders the 'slt' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get_rd functions to get
 * the various fields we need from the insn. Next the values in rs1 and rs2 then 
 * get compared, and if contents of rs1 are less than rs2, rd gets set to 1
 * otherwise it gets set to 0. Next, we call the render_rtype function to render
 * the slt instruction, then print that rendered string and execution of the 
 * instruction. Finally, we set the rd to the value of either 1 or 0 depending on
 * comparison, then increment the pc 4.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_slt(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	uint32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2 (insn);

	//check if the signed value in rs1 is less than the value in rs2, if its less
	//set rd to 1, othewise set it to 0
	int32_t val = (regs.get(rs1) < regs.get(rs2))? 1 : 0;		
	if (pos)
 	{
		//set 's' string to rendered rtype, slt instruction
 		std::string s = render_rtype(insn , "slt ");
 		s.resize(instruction_width , ' ');
		
		//print rendered string and execution of instruction
 		*pos << s << "// x" << std::dec << rd << " = (" << hex0x32(regs.get(rs1)) << " < " << hex0x32(regs.get(rs2))
			<< ") ? 1 : 0 = " << hex0x32(val);
 	}

	//set rd to the value and increment pc + 4
 	regs.set(rd,val);
 	pc += 4;
}
/**
 * Executes and renders the 'sltu' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get_rd functions to get
 * the various fields we need from the insn. Next we static cast the values in rs1
 * and rs2 to make them unsigned, since this is an unsigned instruction. The values
 * in each then get compares, and if contents of rs1 are less than rs2, rd gets set to 1
 * otherwise it gets set to 0. Next, we call the render_rtype function to render
 * the sltu instruction, then print that rendered string and execution of the 
 * instruction. Finally, we set the rd to the value of either 1 or 0 depending on
 * comparison, then increment the pc 4.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_sltu(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2 (insn);

	//check if the UNSIGNED value in rs1 is less than the UNSIGNED value in rs1, if its less
	//set rd to 1, otherwise set it to 0
	uint32_t val = (static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(regs.get(rs2))) ? 1 : 0;
	if (pos)
 	{
		//set 's' string to rendered sltu instruction
 		std::string s = render_rtype(insn , "sltu ");
 		s.resize( instruction_width , ' ');
		
		//print out the rendered string and execution of the instruction
 		*pos << s << "// x" << std::dec << rd << " = (" << hex0x32(regs.get(rs1)) << " <U " << hex0x32(regs.get(rs2))
			<< ") ? 1 : 0 = " << hex0x32(val);
 	}

	//set rd to the value and increment pc + 4
 	regs.set(rd,val);
 	pc += 4;
}
/**
 * Executes and renders the 'jal' instruction
 *
 * This function makes use of the get_rd, and get imm_j functions to
 * get the various fields we need from the insn. After that, we make a temporary
 * pc variable. Then, we set the val to the pc + 4. Pc_val then gets set to pc + 
 * the imm_j value. Next, we call the render_jal instruction, then 
 * print the rendered string and the execution of the instruction. Finally,
 * the program counter gets set to pc_val, and we set the rd to the val calculated.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_jal(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	uint32_t rd = get_rd(insn);
	int32_t imm_j = get_imm_j(insn);

	//set pc_val to program counter val
	uint32_t pc_val = pc;

	//create a variable to hold value
	int32_t val;
		
	//set val to pc + 4
	val = pc + 4;

	//set the pc_val to pc + imm_j value
	pc_val = pc + imm_j;
	
	if (pos)
 	{
		//set 's' string to rendered jal instruction
 		std::string s = render_jal(insn);
 		s.resize( instruction_width , ' ');
		
		//print out rendered string and the execution of the instruction
 		*pos << s << "// x" << std::dec  << rd << " = " << std::hex << hex0x32(val) << ", " << " pc = "  << hex0x32(pc) 
			<< " + " << hex0x32(imm_j) << " = " << hex0x32(pc_val);
 	}

	//set pc equal to pc_val calculated above
	pc = pc_val;

	//set rd register to val calculated above
 	regs.set(rd,val);
}
/**
 * Executes and renders the 'jalr' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get imm_i functions to
 * get the various fields we need from the insn. After that, we make a temporary
 * pc variable. Then, we set the val to the contents of rs1 added to imm_i.
 * Then we create a variable to hold the value of ~1. Val gets set to pc + 4,
 * and pc_val gets set to the contents of rs1 + imm_i, and that gets and-ed with
 * the ~1 variable we created. Next, we call the render_jalr instruction, then 
 * print the rendered string and the execution of the instruction. Finally,
 * the program counter gets set to pc_val, and we set the rd to the val calculated.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_jalr(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//variable that will set pc later
	int32_t pc_val;

	//val is rs1 + imm_i
	int32_t val = (regs.get(rs1) + imm_i);

	//variable to hold ~1 value
	int32_t notOne = 0xfffffffe;
	
	//value that will be stored is program counter value + 4
	val = pc + 4;

	//pc_val is going to set pc later
	//pc_val is contents of rs1 + imm_i and that value is 
	//&-ded with ~1
	pc_val = ((regs.get(rs1) + imm_i) & notOne);

	if (pos)
 	{
		//set 's' string to rendered jalr instruction
 		std::string s = render_jalr(insn);
 		s.resize( instruction_width , ' ');
		
		//print rendered string and execution of the jalr instruction
 		*pos << s << "// x" << std::dec  << rd << " = " << std::hex << hex0x32(val) << ", " << " pc = (" << hex0x32(imm_i)
			<< " + " << hex0x32(regs.get(rs1)) << ") & " << hex0x32(notOne) << " = " << hex0x32(pc_val);
 	}

	//set pc to pc_val we just calculated above and set rd to val
	pc = pc_val;
 	regs.set(rd,val);
}
/**
 * Executes and renders the 'bne' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get imm_b to get the 
 * various fields we need from the insn. After that, we make a temporary pc
 * variable that we can use when we do the comparisons/calculations. After,
 * we compare contents of rs1 to rs2. If rs1 is not equal to the contents of rs2,
 * we will add the imm_b value to pc, else we will add 4 to the pc. Finally, 
 * we print the rendered instruction and the execution of that instruction. 
 * After printing, we will increment the pc counter. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_bne(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need form insn
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	//create a new value to hold addition of pc & other value depending on comparison			
	int32_t pc_val;

	//compare contents of rs1 to rs2, if rs1 is not equal to rs2 add imm_b to pc
	//else add 4
	if (regs.get(rs1) !=  regs.get(rs2))		
	{
		pc_val = pc + imm_b;				//then add imm_b to pc value
	}
	else
	{
		pc_val = pc + 4;				//else add 4 to pc value
	}
	if(pos)
	{
		//set 's' to the rendered bne instruction
		std::string s = render_btype(insn, "bne ");
		s.resize(instruction_width, ' ');

		//print the rendered string and execution of the instruction
		*pos << s << "// pc += (" << hex0x32(regs.get(rs1)) << " != " << hex0x32(regs.get(rs2)) << " ? " << hex0x32(imm_b) 
			<< " : " << "4) = " << hex0x32(pc_val);
	}

	//set pc equal to the pc_value we calculated above
	pc = pc_val;
}
/**
 * Executes and renders the 'beq' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get imm_b to get the 
 * various fields we need from the insn. After that, we make a temporary pc
 * variable that we can use when we do the comparisons/calculations. After,
 * we compare contents of rs1 to rs2. If rs1 is equal to the contents of rs2,
 * we will add the imm_b value to pc, else we will add 4 to the pc. Finally, 
 * we print the rendered instruction and the execution of that instruction. 
 * After printing, we will increment the pc counter. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_beq(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	//create a new value to hold addition of pc & other value depending on comparison			
	int32_t pc_val;

	//compare the contents of rs1 and rs2, if equal then add imm_b to pc
	//else add 4
	if (regs.get(rs1) ==  regs.get(rs2))			
	{
		pc_val = pc + imm_b;				//then add imm_b to pc value
	}
	else
	{
		pc_val = pc + 4;				//add 4 to pc value
	}
	if(pos)
	{
		//set 's' string to the rendered beq instruction
		std::string s = render_btype(insn, "beq ");
		s.resize(instruction_width, ' ');

		//print the rendered string and the execution of the instruction
		*pos << s << "// pc += (" << hex0x32(regs.get(rs1)) << " == " << hex0x32(regs.get(rs2)) << " ? " << hex0x32(imm_b) 
			<< " : " << "4) = " << hex0x32(pc_val);
	}

	//set pc equal to the pc_val we calculated above
	pc = pc_val;
}
/**
 * Executes and renders the 'blt' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get imm_b to get the 
 * various fields we need from the insn. After that, we make a temporary pc
 * variable that we can use when we do the comparisons/calculations. After,
 * we compare contents of rs1 to rs2. If rs1 is less than the contents of  rs2,
 * we will add the imm_b value to pc, else we will add 4 to the pc. Finally, 
 * we print the rendered instruction and the execution of that instruction. 
 * After printing, we will increment the pc counter. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_blt(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	//create a new value to hold addition of pc & other value depending on comparison		
	int32_t pc_val;

	//compare the contents of rs1 to rs2, if rs1 is less than rs2 add imm_b to pc
	//else add 4
	if (regs.get(rs1) <  regs.get(rs2))		
	{
		pc_val = pc + imm_b;				//then add imm_b to pc value
	}
	else
	{
		pc_val = pc + 4;				//else add 4
	}
	if(pos)
	{
		//set 's' string to rendered blt instruction
		std::string s = render_btype(insn, "blt ");
		s.resize(instruction_width, ' ');

		//print the rendered string and the execution of the instruction
		*pos << s << "// pc += (" << hex0x32(regs.get(rs1)) << " < " << hex0x32(regs.get(rs2)) << " ? " << hex0x32(imm_b) 
			<< " : " << "4) = " << hex0x32(pc_val);
	}

	//set pc to the pc_val we calculated above
	pc = pc_val;
}
/**
 * Executes and renders the 'bge' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get imm_b to get the 
 * various fields we need from the insn. After that, we make a temporary pc
 * variable that we can use when we do the comparisons/calculations. After,
 * we compare contents of rs1 to rs2. If rs1 is greater than or equal to rs2,
 * we will add the imm_b value to pc, else we will add 4 to the pc. Finally, 
 * we print the rendered instruction and the execution of that instruction. 
 * After printing, we will increment the pc counter. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_bge(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	//create a new value to hold addition of pc & other value depending on comparison	
	int32_t pc_val;

	//compare contents of rs1 to rs2, if rs1 is greater than or equal, add imm_b to pc
	//else add 4
	if (regs.get(rs1) >= regs.get(rs2))			
	{
		pc_val = pc + imm_b;				//then add imm_b to pc value
	}
	else
	{
		pc_val = pc + 4;				//else add 4
	}
	if(pos)
	{
		//set 's' to the rendered bge instruction
		std::string s = render_btype(insn, "bge ");
		s.resize(instruction_width, ' ');

		//print the rendered string and the execution of the instruction
		*pos << s << "// pc += (" << hex0x32(regs.get(rs1)) << " >= " << hex0x32(regs.get(rs2)) << " ? " << hex0x32(imm_b) 
			<< " : " << "4) = " << hex0x32(pc_val);
	}

	//set pc equal to the pc_val we calculated above
	pc = pc_val;
}
/**
 * Executes and renders the 'bltu' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get imm_b to get the 
 * various fields we need from the insn. After that, we make a temporary pc
 * variable that we can use when we do the comparisons/calculations. After,
 * we static cast the contents of rs1 and rs2 to be unsigned so that we can
 * compare rs1 to rs2. If rs1 is less than  rs2, we will add the
 * imm_b value to pc, else we will add 4 to the pc. Finally, we print the rendered
 * instruction and the execution of that instruction. After printing, we will 
 * increment the pc counter. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_bltu(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	//create a new value to hold addition of pc & other value depending on comparison
	int32_t pc_val;

	//static_cast rs1 and rs2 to be unsigned since this is an unsigned instruction, then compare
	//the contents of rs1 to rs2, if rs1 is less than rs2, add imm_b to pc, else add 4 
	if (static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(regs.get(rs2)))		
	{
		pc_val = pc + imm_b;				//then add imm_b to pc value
	}
	else
	{
		pc_val = pc + 4;				//otherwise add 4
	}
	if(pos)
	{
		// set 's' string to rendered bltu instruction
		std::string s = render_btype(insn, "bltu ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the bltu instruction
		*pos << s << "// pc += (" << hex0x32(regs.get(rs1)) << " <U " << hex0x32(regs.get(rs2)) << " ? " << hex0x32(imm_b) 
			<< " : " << "4) = " << hex0x32(pc_val);
	}

	//set pc equal to the pc_val we calculated above
	pc = pc_val;
}
/**
 * Executes and renders the 'bgeu' instruction
 *
 * This function makes use of the get_rs1, get_rs2, and get imm_b to get the 
 * various fields we need from the insn. After that, we make a temporary pc
 * variable that we can use when we do the comparisons/calculations. After,
 * we static cast the contents of rs1 and rs2 to be unsigned so that we can
 * compare rs1 to rs2. If rs1 is greater than or equal to rs2, we will add the
 * imm_b value to pc, else we will add 4 to the pc. Finally, we print the rendered
 * instruction and the execution of that instruction. After printing, we will 
 * increment the pc counter. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_bgeu(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);

	//create a new value to hold addition of pc & other value depending on comparison
	int32_t pc_val;
	
	//static_cast rs1 and contents of rs2 to be unsigned since this is an unsigned instruction
	//then compare, it rs1 is greater than or equal, we will add imm_b calue to pc
	//else we add 4
	if (static_cast<uint32_t>(regs.get(rs1)) >= static_cast<uint32_t>(regs.get(rs2)))	
	{
		pc_val = pc + imm_b;				//then add imm_b to pc value
	}
	else
	{
		pc_val = pc + 4;				// otherwise add 4
	}
	if(pos)
	{
		//set 's' string to rendered bgeu instruction
		std::string s = render_btype(insn, "bgeu ");
		s.resize(instruction_width, ' ');

		//print out rendered string and the execution of the bgeu instruction
		*pos << s << "// pc += (" << hex0x32(regs.get(rs1)) << " >=U " << hex0x32(regs.get(rs2)) << " ? " << hex0x32(imm_b) 
			<< " : " << "4) = " << hex0x32(pc_val);
	}

	//set pc equal to the pc_val we calculated above 
	pc = pc_val;
}
/**
 * Executes and renders the 'addi' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. After we add the contents of rs1 and imm_i 
 * together to get the val. Finally, we call the render_itype_alu function to render
 * the addi instruction, and then print out the rendering of the instruction
 * as well as the execution of it. Lastly, we increment the pc counter and set
 * rd to the val calculated above by adding rs1 and imm_i
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_addi(uint32_t insn, std::ostream* pos)
{
	//get various fields we need
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//value that will be set in rd later is rs1 contents + imm_i
	int32_t val = regs.get(rs1) + imm_i;
	if(pos)
	{
		//set 's' string to rendered addi instrcution
		std::string s = render_itype_alu(insn, "addi ", imm_i);
		s.resize(instruction_width, ' ');

		//print the rendered string and the execution of the instrcution
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i) << " = " << hex0x32(val);
	}

	//increment the pc counter and set rd to the val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'slti' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. Next, we compare contents of rs1  and
 * imm_i together, if rs1 is less than imm_i then we set rd to 1, else we
 * set rd to 0. Finally, we call the render_itype_alu function to render
 * the slti instruction, and then print out the rendering of the instruction
 * as well as the execution of it. Lastly, we increment the pc counter. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_slti(uint32_t insn, std::ostream* pos)
{	
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	int32_t val = regs.get(rs1);
	//if the contents of rs1 are less than imm_i, set rd to 1
	//else set rd to 0
	if(regs.get(rs1) < imm_i)
	{
		regs.set(rd,1);
	}
	else
	{
		regs.set(rd,0);
	}
	if(pos)
	{
		//set 's' string to rendered slti instruction
		std::string s = render_itype_alu(insn, "slti ", imm_i);
		s.resize(instruction_width, ' ');
		
		//print out the rendered string and the execution of the instruction
		*pos << s << "// x" << std::dec << rd  << " = ("  << hex0x32(val) << " < " << std::dec << (imm_i) << ") ? 1 : 0 = " << hex0x32(regs.get(rd));
	}

	//increment the pc counter
	pc += 4;
}
/**
 * Executes and renders the 'sltiu' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. After we get the various fields, we set the 
 * contents of rs1 to 'val' variable. Next, we change 'val' and 'imm_i' variable
 * to be unsigned since this is an unsigned instruction. We compare val and
 * imm_i together, if val is less than imm_i then we set rd to 1, else we
 * set rd to 0. Finally, we call the render_itype_alu function to render
 * the sltiu instruction, and then print out the rendering of the instruction
 * as well as the execution of it. Lastly, we increment the pc counter. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_sltiu(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);

	//set value to the contents of rs1
	uint32_t val = regs.get(rs1);

	//change val and imm_i to be unsigned, since unsigned instruction
	//and compare val to imm_i.. if val is less set rd to 1, else
	//set rd to 0
	if(static_cast<uint32_t>(val) < static_cast<uint32_t>(imm_i))		
	{
		regs.set(rd,1);				//set rd to 1
	}
	else
	{
		regs.set(rd,0);				//otherwise set it to 0
	}
	if(pos)
	{
		//set 's' string to rendered sltiu instruction
		std::string s = render_itype_alu(insn, "sltiu ", imm_i);
		s.resize(instruction_width, ' ');
		
		//print rendered string and execution of instruction
		*pos << s << "// x" << std::dec << rd << " = (" << hex0x32(val) << " <U " << std::dec << (imm_i) << ") ? 1 : 0 = " << hex0x32(regs.get(rd));
	}

	//increment pc counter
	pc += 4;
}
/**
 * Executes and renders the 'xori' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. After we get the various fields, we 'xor' the 
 * value in rs1 with the imm_i value. Then, we print out the rendered string (that
 * we got from calling the render_itype_alu function) and execution of the 
 * instruction. Lastly, we increment the pc counter and store the value in rd.  
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_xori(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);

	//'xor' the value in rs1 with imm_i value
	uint32_t val = ((regs.get(rs1)) ^ imm_i);	
	
	if(pos)
	{
		//set 's' string to rendered xori instruction
		std::string s = render_itype_alu(insn, "xori ", imm_i);
		s.resize(instruction_width, ' ');
		
		//print out the rendered string and execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " ^ " << hex0x32(imm_i) << " = " << hex0x32(val);
	}

	//increment pc counter and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'ori' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. After we get the various fields, we 'or' the 
 * value in rs1 with the imm_i value. Then, we print out the rendered string (that
 * we got from calling the render_itype_alu function) and execution of the 
 * instruction. Lastly, we increment the pc counter and store the value in rd.  
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_ori(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);

	//'or' the value in rs1 with the imm_i value
	uint32_t val = ((regs.get(rs1)) | imm_i);

	if(pos)
	{
		//set 's' string to rendered ori instruction
		std::string s = render_itype_alu(insn, "ori ", imm_i);
		s.resize(instruction_width, ' ');
		
		//print out the rendered string and exeuction of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " | " << hex0x32(imm_i) << " = " << hex0x32(val);
	}

	//increment pc counter and then set rd to the val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'andi' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. After we get the various fields, we 'and' the 
 * value in rs1 with the imm_i value. Then, we print out the rendered string
 * and execution of the instruction. Lastly, we increment the pc counter
 * and store the value in rd.  
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_andi(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//'and' the value in rs1 with the imm_i value
	int32_t val = ((regs.get(rs1)) & imm_i);
	
	if(pos)
	{
		//set 's' string to the rendering of the andi instruction
		std::string s = render_itype_alu(insn, "andi ", imm_i);
		s.resize(instruction_width, ' ');
		
		//print the rendered string and the exeuction of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " & " << hex0x32(imm_i) << " = " << hex0x32(val);
	}

	//increment the pc counter and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'slli' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. After we get the various fields, we calculate the
 * shift amount by zeroing out the left 27 bits to get the least 5 bits. These hold
 * the shift amount. After we have the shift amount, we will shift the value in
 * rs1, then print out the rendering, execution of the instruction, and finally
 * increment the pc and then set rd to the value. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_slli(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);


	//calculate the shamt_i
	uint32_t shamt_i = (imm_i << 27);	//shift 27 to zero out those bits
	shamt_i >>= 27;				//shift back
	shamt_i = shamt_i & 0xfffffe1f;		//mask to get last 
	
	//shift the value by shift amount
	int32_t val = ((regs.get(rs1)) << shamt_i);
	
	if(pos)
	{
		//set 's' string to rendering of slli instruction
		std::string s = render_itype_alu_shamt(insn, "slli ", imm_i % XLEN);
		s.resize(instruction_width, ' ');
		
		//print rendered string and execution of instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " << " << std::dec << shamt_i  << " = " << hex0x32(val);
	}

	//increment pc counter and set rd to the value
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'srli' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. After we get the various fields, we calculate the
 * shift amount by zeroing out the left 27 bits to get the least 5 bits. These hold
 * the shift amount. After we have the shift amount, we create a mask so that any
 * bits that were shifted off, get filled in with zeroes. Then we shift, apply the 
 * mask, and print out the rendering and execution. Finally, we increment the pc
 * counter and set the rd to the value. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_srli(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t imm_i = get_imm_i(insn);

	//calculate the shamt_i
	uint32_t shamt_i = (imm_i << 27);	//shift 27 to zero out those bits
	shamt_i >>= 27;				//shift back
	shamt_i = shamt_i & 0xfffffe1f;		//mask to get last 

	//the following 3 lines set up the mask to get zeros filled in when shifting
	int32_t mask = 0x80000000;
	mask = mask >> shamt_i;
	mask = ~(mask << 1);
	
	//shift the value by shamt_i and then mask to get zeroes filled in 
	uint32_t val = ((regs.get(rs1)) >> shamt_i);
	val = val & mask;
	
	if(pos)
	{
		//set 's' string to the rendered srli instruction
		std::string s = render_itype_alu_shamt(insn, "srli ", imm_i % XLEN);
		s.resize(instruction_width, ' ');
		
		//print the rendered string and execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " >> " << std::dec << shamt_i  << " = " << hex0x32(val);
	}

	//increment the pc counter and set rd to the logically shifted value
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'srai' instruction
 *
 * This function makes use of the get_rs1, get_rd, and get_imm_i functions to 
 * obtain the fields we need. After we get the various fields, we calculate the
 * shift amount by zeroing out the left 27 bits to get the least 5 bits. These hold
 * the shift amount. After we have the shift amount, we shift the contents of
 * rs1 by the shift amount (since this is an arithmetic shift, we will fill in with 
 * a signed bit), the print the rendering and execution, and finally
 * increment the pc counter, and set the rd to the shifted value. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_srai(uint32_t insn, std::ostream* pos)
{
	//get the various fields from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);
	
	//calculate the shamt_i
	uint32_t shamt_i = (imm_i << 27);	//shift 27 to zero out those bits
	shamt_i >>= 27;				//shift back
	shamt_i = shamt_i & 0xfffffe1f;		//mask to get last 

	//the value we store is the contents of rs1 shifted right arithmatically
	//by the shamt_i amount just calculated
	int32_t val = ((regs.get(rs1)) >> shamt_i);
	
	if(pos)
	{
		//set 's' string to the rendered srai instruction
		std::string s = render_itype_alu_shamt(insn, "srai ", imm_i % XLEN);
		s.resize(instruction_width, ' ');
		
		//print the rendered string and exeuction of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " >> " << std::dec << shamt_i  << " = " << hex0x32(val);
	}

	//increment the pc counter and store the shifted amount in rd
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'lbu' instruction
 *
 * This function makes use of the get_rs1, and get_rs2 functions to get
 * the value of the those registers, as well as get_imm_i to get the imm_i
 * value. Next, the address we will obtain the 8-bit value in memory is computed by 
 * adding the value in rs1 and the imm_i value. After that, we make use of the 
 * get32 memory class function to obtain the 8-bit little-endian value from the 
 * address. Then, we print the rendering and execution line, increment the pc
 * counter, and set the rd to the 8-bit unsigned little endian value.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_lbu(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//the address we fetch the 8-bits from is the contents of rs1 + imm_i
	int32_t address = regs.get(rs1) + imm_i;

	//get the 8-bits fromo the address
	uint32_t byte = mem->get8(address);

	if(pos)
	{
		//set 's' string to the rendering of the lbu instruction
		std::string s = render_itype_load(insn, "lbu ");
		s.resize(instruction_width, ' ');

		//print the rendered string and execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = zx(m8(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i)
		       << "))" << " = " << hex0x32(byte);
	}

	//increment the pc and then set rd to the byte
	pc += 4;
	regs.set(rd,byte);
}
/**
 * Executes and renders the 'lhu' instruction
 *
 * This function makes use of the get_rs1, and get_rs2 functions to get
 * the value of the those registers, as well as get_imm_i to get the imm_i
 * value. Next, the address we will obtain the 16-bit value in memory is computed by 
 * adding the value in rs1 and the imm_i value. After that, we make use of the 
 * get32 memory class function to obtain the 16-bit little-endian value from the 
 * address. Then, we print the rendering and execution line, increment the pc
 * counter, and set the rd to the 16-bit unsigned little endian value.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_lhu(uint32_t insn, std::ostream* pos)
{
	//get the various fields we know from insn
	int32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//the address we fetch 16-bits from is the contents of rs1 + imm_i
	int32_t address = regs.get(rs1) + imm_i;

	//get the 16-bits from the address
	uint32_t sixteen = mem->get16(address);

	if(pos)
	{
		//set 's' string to the rendering of the lhu instruction
		std::string s = render_itype_load(insn, "lhu ");
		s.resize(instruction_width, ' ');

		//print the rendered string and execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = zx(m16(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i)
		       << "))" << " = " << hex0x32(sixteen);
	}

	//increment the pc counter and then set the register to the 16 bits
	pc += 4;
	regs.set(rd,sixteen);
}
/**
 * Executes and renders the 'lb' instruction
 *
 * This function makes use of the get_rs1, and get_rs2 functions to get
 * the value of the those registers, as well as get_imm_i to get the imm_i
 * value. Next, the address we will obtain the 8-bit value in memory is computed by 
 * adding the value in rs1 and the imm_i value. After that, we make use of the 
 * get32 memory class function to obtain the 8-bit little-endian value from the 
 * address. Then, we print the rendering and execution line, increment the pc
 * counter, and set the rd to the sign- extended 8-bit little endian value.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_lb(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//the address we fetch 8-bits from is the contents of rs1 + imm_i
	int32_t address = regs.get(rs1) + imm_i;

	//get the 8-bits from the address	
	uint32_t byte = mem->get8(address);					

	//sign extend it
	if(byte & 0x0000080)
		byte |= 0xffffff00;
	if(pos)
	{
		//set 's' string to the rendering of the lb instruction
		std::string s = render_itype_load(insn, "lb ");
		s.resize(instruction_width, ' ');

		//print the rendered string and the execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = sx(m8(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i)
		       << "))" << " = " << hex0x32(byte);
	}

	//increment pc by 4 & then set the register to that byte
	pc += 4;
	regs.set(rd,byte);
}
/**
 * Executes and renders the 'lh' instruction
 *
 * This function makes use of the get_rs1, and get_rs2 functions to get
 * the value of the those registers, as well as get_imm_i to get the imm_i
 * value. Next, the address we will obtain the 16-bit value in memory is computed by 
 * adding the value in rs1 and the imm_i value. After that, we make use of the 
 * get32 memory class function to obtain the 16-bit little-endian value from the 
 * address. Then, we print the rendering and execution line, increment the pc
 * counter, and set the rd to the sign-extended 16-bit little endian value.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_lh(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//the address we fetch the 16 bits from is contents of rs1 + imm_i
	int32_t address = regs.get(rs1) + imm_i;

	//get the 16-bits from the address
	uint32_t sixteen = mem->get16(address);

	//sign extend it
	if(sixteen & 0x0008000)
		sixteen |= 0xffff0000;
	if(pos)
	{
		//set 's' string to the rendering of the lh instruction
		std::string s = render_itype_load(insn, "lh ");
		s.resize(instruction_width, ' ');

		//print the rendered strng and the execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = sx(m16(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i)
		       << "))" << " = " << hex0x32(sixteen);
	}

	//increment pc counter and set the register to the 16 bits
	pc += 4;
	regs.set(rd,sixteen);
}
/**
 * Executes and renders the 'lw' instruction
 *
 * This function makes use of the get_rs1, and get_rs2 functions to get
 * the value of the those registers, as well as get_imm_i to get the imm_i
 * value. Next, the address we will obtain the 32-bit value in memory is computed by 
 * adding the value in rs1 and the imm_i value. After that, we make use of the 
 * get32 memory class function to obtain the 32-bit little-endian value from the 
 * address. Then, we print the rendering and execution line, increment the pc
 * counter, and set the rd to the 32-bit little endian value.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_lw(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);

	//the address  we fetch 32 bit from is contents of rs1 + imm_i
	int32_t address = regs.get(rs1) + imm_i;

	//get the 32-bits from the address
	uint32_t thirty_two = mem->get32(address);

	//sign extend it
	if(thirty_two & 0x80000000)
		thirty_two |= 0x00000000;
	if(pos)
	{
		//set 's' string to the rendering of the lw instruction
		std::string s = render_itype_load(insn, "lw ");
		s.resize(instruction_width, ' ');

		//print the rendered string and the execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = sx(m32(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_i)
		       << "))" << " = " << hex0x32(thirty_two);
	}

	//increment the pc counter and set the rd to the 32 bits
	pc += 4;
	regs.set(rd,thirty_two);
}
/**
 * Executes and renders the 'sb' instruction
 *
 * This function makes use of the get_rs1, and get_rs2 functions to get
 * the value of the those registers, as well as get_imm_s to get the imm_s
 * value. Next, the address we will set the 8-bit value in memory at is computed by 
 * adding the value in rs1 and the imm_s value. We then print the exeuction of the
 * instruction, increment the pc counter, then set the memory at the address to
 * val (which is the least significant 8 bits in rs2).
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_sb(uint32_t insn, std::ostream* pos)
{	

	//get the various fields we need from insn
	int32_t rs2 = get_rs2(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_s = get_imm_s(insn);

	//value that will get set at the address in memory (before extraction)
        int32_t val = (regs.get(rs2));

	//the address we want to set in memory is value in rs1 + imm_s value
 	int32_t address = regs.get(rs1) + imm_s;		

	//extract 8 LSB bits of rs2
	val = (val & 0x000000ff);

	if(pos)
	{
		//set 's' string to the rendering of the sb instruction
		std::string s = render_stype(insn, "sb ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the instruction
		*pos << s << "// m8(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_s)
		       << ")" << " = " << hex0x32(val);						
	}

	//increment pc counter and set the address to val
	pc += 4;
	mem->set8(address,val);					
}
/**
 * Executes and renders the 'sh' instruction
 *
 * This function makes use of the get_rs1, and get_rs2 functions to get
 * the value of the those registers, as well as get_imm_s to get the imm_s
 * value. Next, the address we will set the 16-bit value in memory at is computed by 
 * adding the value in rs1 and the imm_s value, then masked to ensure we get the
 * 16 LSB's. We then print the exeuction of the instruction, increment the pc 
 * counter, then set the memory at the address to val (which is the least sig
 * 16 bits in rs2)
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_sh(uint32_t insn, std::ostream* pos)
{	
	//get the various fields we need from insn
	int32_t rs2 = get_rs2(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_s = get_imm_s(insn);

	//value that will get set at the address in memory (before extraction)
	int32_t val = (regs.get(rs2));

	//the address we want to set is value in rs1 + imm_s value
	int32_t address = regs.get(rs1) + imm_s;
	
	//extract 16 LSB bits of rs2
	val = (val & 0x0000ffff);

	if(pos)
	{
		//set 's' string to the rendering of the sh instruction
		std::string s = render_stype(insn, "sh ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the instruction
		*pos << s << "// m16(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_s)
		       << ")" << " = " << hex0x32(val);						 
	}

	//increment pc counter and set the address to val
	pc += 4;
	mem->set16(address,val);
}
/**
 * Executes and renders the 'sw' instruction
 *
 * This function makes use of the get_rs1, and get_rs2 functions to get
 * the value of the those registers, as well as get_imm_s to get the imm_s
 * value. Next, the address we will set the 32-bit value in memory at is computed by 
 * adding the value in rs1 and the imm_s value. We then print the exeuction of the
 * instruction, increment the pc counter, then set the memory at the address to
 * val.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_sw(uint32_t insn, std::ostream* pos)
{	
	//get the various fields we need from the insn
	int32_t rs2 = get_rs2(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t imm_s = get_imm_s(insn);

	//the address we want to set is value in rs1 + imm_s value
	int32_t address = regs.get(rs1) + imm_s;

	//value that will get set at the address computed above
        int32_t val = (regs.get(rs2));
	
	if(pos)
	{
		//set 's' string to the rendering of the sw instruction
		std::string s = render_stype(insn, "sw ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the instruction
		*pos << s << "// m32(" << hex0x32(regs.get(rs1)) << " + " << hex0x32(imm_s)
		       << ")" << " = " << hex0x32(val);						 
	}

	//increment pc counter and set address to val
	pc += 4;
	mem->set32(address,val);
}
/**
 * Executes and renders the 'add' instruction
 *
 * This function makes use of the get_rd, get_rs1, and get_rs2 functions to get
 * the value of the those registers. It then uses the registerfile class function
 * get(r) to get the value in the registers of rs2 and rs1 and '+'s them together.
 * That value is then stored in the rd register after we print out the execution
 * line and increment the program counter.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_add(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);

	//add together contents of rs1 and rs2
	int32_t val = (regs.get(rs1) + regs.get(rs2));
	
	if(pos)
	{
		//set 's' string to the rendering of the add function
		std::string s = render_rtype(insn, "add ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the exeuction of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " + " << hex0x32(regs.get(rs2)) 
			<< " = " << hex0x32(val);	
	}

	//increment pc counter and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'sub' instruction
 *
 * This function makes use of the get_rd, get_rs1, and get_rs2 functions to get
 * the value of the those registers. It then uses the registerfile class function
 * get(r) to get the value in the registers of rs2 and rs1 and '-'s them together.
 * That value is then stored in the rd register after we print out the execution
 * line and increment the program counter.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_sub(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);

	//subtract contents of rs2 from rs1
	int32_t val = (regs.get(rs1) - regs.get(rs2));
	
	if(pos)
	{
		//set 's' string to the rendering of subtraction instruction
		std::string s = render_rtype(insn, "sub ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " - " << hex0x32(regs.get(rs2)) 
			<< " = " << hex0x32(val);	
	}

	//increment pc counter and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'xor' instruction
 *
 * This function makes use of the get_rd, get_rs1, and get_rs2 functions to get
 * the value of the those registers. It then uses the registerfile class function
 * get(r) to get the value in the registers of rs2 and rs1 and '^'s them together.
 * That value is then stored in the rd register after we print out the execution
 * line and increment the program counter.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_xor(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);

	//xor the contents of rs1 and rs2
	int32_t val = (regs.get(rs1) ^ regs.get(rs2));
	
	if(pos)
	{
		//set 's' string to the rendering of the xor instruction
		std::string s = render_rtype(insn, "xor ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " ^ " << hex0x32(regs.get(rs2)) 
			<< " = " << hex0x32(val);	
	}

	//increment pc counter and set value of rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'or' instruction
 *
 * This function makes use of the get_rd, get_rs1, and get_rs2 functions to get
 * the value of the those registers. It then uses the registerfile class function
 * get(r) to get the value in the registers of rs2 and rs1 and '|'s them together.
 * That value is then stored in the rd register after we print out the execution
 * line and increment the program counter.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_or(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);

	//'or' the contents of rs1 and rs2 together
	int32_t val = (regs.get(rs1) | regs.get(rs2));
	
	if(pos)
	{
		//set 's' string to the rendering of the or function
		std::string s = render_rtype(insn, "or ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " | " << hex0x32(regs.get(rs2)) 
			<< " = " << hex0x32(val);	
	}

	//increment pc counter and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the 'and' instruction
 *
 * This function makes use of the get_rd, get_rs1, and get_rs2 functions to get
 * the value of the those registers. It then uses the registerfile class function
 * get(r) to get the value in the registers of rs2 and rs1 and '&'s them together.
 * That value is then stored in the rd register after we print out the execution
 * line and increment the program counter.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_and(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);

	//'and' the contents of rs1 and rs2 together
	int32_t val = (regs.get(rs1) & regs.get(rs2));
	
	if(pos)
	{
		//set 's' to the rendering of the and function
		std::string s = render_rtype(insn, "and ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(regs.get(rs1)) << " & " << hex0x32(regs.get(rs2)) 
			<< " = " << hex0x32(val);	
	}

	//increment pc and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the shift left logical instruction
 *
 * This function makes use of the get_rd, get_rs1, and get_rs2 functions to get
 * the various fields from the insn. It then uses the registerfile class function
 * get(r) to get the value in the registers of rs2 and rs1. It then masks to 
 * get the least 5 bits in the rs2 register and then shifts the rs1 memory by 
 * those bits. Finally, it prints the execution line, increments the program 
 * counter, and sets the rd to the shifted value.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_sll(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn	
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);

	int32_t rs2mem = (regs.get(rs2));			//get the memory in rs2
	int32_t rs1mem = (regs.get(rs1));			//get the memory in rs1
	rs2mem = (rs2mem & 0x0000001f);				//mask to get least 5 bits in rs2 memory
	int32_t val = (rs1mem << rs2mem);			//shift rs1 memory by least 5 bits of rs2

	if(pos)
	{
		//set 's' to the rendering of the sll function
		std::string s = render_rtype(insn, "sll ");	
		s.resize(instruction_width, ' ');

		//print the rendered string and then the exeuction of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(rs1mem) << " << " << std::dec << (rs2mem) 
			<< " = " << hex0x32(val);	
	}

	//increment the pc counter and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the shift right logical instruction
 *
 * This function makes use of the get_rd, get_rs1, and get_rs2 functions to get
 * the value of the those registers. It then uses the registerfile class function
 * get(r) to get the value in the registers of rs2 and rs1. It then, masks to 
 * get the least 5 bits in the rs2 register and then shifts the rs1 memory by 
 * thse bits. Next, it creates a mask so that the amount of bits that get shifted 
 * off get filled in with 0's since this is a logical shift.  Finally, it prints 
 * the execution line, increments the program counter, and sets the rd to the 
 * shifted value.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_srl(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);

	int32_t rs2mem = (regs.get(rs2));			//get the memory in rs2
	int32_t rs1mem = (regs.get(rs1));			//get the memory in rs1

	//the following 4 lines set up the mask to get zeros filled in when shifting
	int32_t mask = 0x80000000;
	rs2mem = (rs2mem & 0x0000001f);				//mask to get least 5 bits in rs2 memory
	mask = mask >> rs2mem;
	mask = ~(mask << 1);

	//value to store is rs1 value shifted by last 5 bits in rs2 and 
	//mask that value so we fill in the shifted bits with 0's
	int32_t val = (rs1mem >> rs2mem);			
	val = val & mask;

	if(pos)
	{
		//set 's' to the rendering of the srl instruction
		std::string s = render_rtype(insn, "srl ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the execution of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(rs1mem) << " >> " << std::dec << (rs2mem) 
			<< " = " << hex0x32(val);	
	}

	//increment the pc counter and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Executes and renders the shift right arithmetic  instruction
 *
 * This function makes use of the get_rd, get_rs1, and get_rs2 functions to get
 * the value of the those registers. It then uses the registerfile class function
 * get(r) to get the value in the registers of rs2 and rs1. It then, masks to 
 * get the least 5 bits in the rs2 register and then shifts the rs1 memory by 
 * thse bits. Finally, it prints the execution line, increments the program
 * counter, and sets the rd to the shifted value.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_sra(uint32_t insn, std::ostream* pos)
{
	//get the various fields we need from insn
	int32_t rd = get_rd(insn);
	int32_t rs1 = get_rs1(insn);
	int32_t rs2 = get_rs2(insn);

	int32_t rs2mem = (regs.get(rs2));			//get the memory in rs2
	int32_t rs1mem = (regs.get(rs1));			//get the memory in rs1
	rs2mem = (rs2mem & 0x0000001f);				//mask to get least 5 bits in rs2 memory
	int32_t val = (rs1mem >> rs2mem);			//shift rs1 memory by least 5 bits of rs2

	if(pos)
	{
		//set 's' to the rendering of the sra instruction
		std::string s = render_rtype(insn, "sra ");
		s.resize(instruction_width, ' ');

		//print the rendered string and then the exeuction of the instruction
		*pos << s << "// x" << std::dec << rd << " = " << hex0x32(rs1mem) << " >> " << std::dec << (rs2mem) 
			<< " = " << hex0x32(val);	
	}

	//increment the pc counter and set rd to val
	pc += 4;
	regs.set(rd,val);
}
/**
 * Function that will reset the registers to their default value, set the program
 * counter to 0, set the insn_counter to 0, and halt to false
 ********************************************************************************/
void rv32i::reset()
{
	regs.reset();		//reset registers to default value
	pc = 0;			//reset program counter to 0
	insn_counter = 0;	//reset insn_counter to 0
	halt = false;		//reset halt to false
}
/**
 * Executes and renders the ebreak instruction
 *
 * This function is used to set the halt flag. Once an ebreak instruction is
 * found, the halt flag will be set to true. It also makes use of the 
 * render_ebreak function to print out the name of it.
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_ebreak( uint32_t insn, std::ostream* pos)
{
	if (pos)
 	{
 		std::string s = render_ebreak (insn);		//call to render_ebreak function
 		s.resize(instruction_width , ' ');
 		*pos << s << "// HALT";			//print HALT since we stop executing with ebreak
 	}

	//set halt to true, so we stop executing
	halt = true;	
}
/**
 * Executes and renders the fence instruction
 *
 * This function is used to output the rendering of fence, as well as incrementing
 * the pc counter by 4. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_fence( uint32_t insn, std::ostream* pos)
{
	if (pos)
 	{
 		std::string s = render_fence(insn);		//call to render_fence function
 		s.resize(instruction_width , ' ');
 		*pos << s << "// fence";		
 	}

	//increment the pc by 4
	pc+= 4;		
}
/**
 * Executes and renders the ecall instruction
 *
 * This function is used to output the rendering of ecall, as well as incrementing
 * the pc counter by 4. 
 *
 * @param insn	Instruction to decode and execute 
 * @param pos   Output stream object used to print execution
 * *****************************************************************************/
void rv32i::exec_ecall( uint32_t insn, std::ostream* pos)
{
	if (pos)
 	{
 		std::string s = render_ecall(insn);		//call to render_ecall function
 		s.resize(instruction_width , ' ');
 		*pos << s << "// ecall";
 	}

	//increment the pc by 4
	pc+= 4;		
}
/**
 * Function that will return if the halt flag is set. If it is not, it will
 * simulate the execution of one single instruction by:
 * 	- incrementing the instruction counter
 * 	- dump the state of the registers if show_registers is true
 * 	- fetch an instruction from memory at pc
 * 	- if show instruction is true, it will print the value of the pc, print
 * 	the value of the instruction, then execute and render it
 * 	- if show instructions is false, we will still execute the instructions, 
 * 	but not print them to output
 ********************************************************************************/
void rv32i::tick()
{
	uint32_t insn;

	//if halt is true, then just return
	if(halt == true)
	{
		return;
	}
	else
	{
		insn_counter += 1;				//increment up the insns_counter

		//if show_registers is set to true
		if(show_registers == true)
		{
			dump();					//dump the state of the registers
		}

		//fetch an instruction from memory at pc
		insn = mem->get32(pc);
		if(show_instructions == true)
		{
			std::cout << hex32(pc) << ": ";		//printing value of pc register
			std::cout << hex32(insn) << "  ";	//printing value of instruction
			dcex(insn, &std::cout);			//execute intruction and render it
			std::cout << std::endl;
		}
		else
		{
			dcex(insn, nullptr);
		}
	}
}
/**
 * Function that will dump the state of the registers, by calling the dump()
 * method from the registerfile class. After it dumps the registers, it will 
 * print out the value of the program counter
 ********************************************************************************/
void rv32i::dump() const
{
	regs.dump();										//dump the contents of the GP-registers
	std::cout << std::endl;
	std::cout << std::right << std::setw(4) << "pc " << hex32(pc) << "\n";			//dump the contents of the pc register
}
/**
 * Setter for member variable, show_instructions
 *
 * @param b	Bool variable that show_instructions will be set to.
 ********************************************************************************/
void rv32i::set_show_instructions(bool b)
{
	show_instructions = b;
}
/**
 * Setter for member variable, show_registers
 *
 * @param b	Bool variable that show_registers will be set to.
 ********************************************************************************/
void rv32i::set_show_registers(bool b)
{
	show_registers = b;
}
/**
 * This function will go through and call tick() while halt is false, or until
 * the limit is reached.
 *
 * This run() function makes use of get_size() from the memory class to get the 
 * memory size and set register 2 to that value. After, it calls tick() until
 * halt == true, or we reach the limit value. If we stopped do to halt being
 * true, we will output that that is the reason why we stopped, but regardless
 * we will print the number of instructions executed. 
 *
 * @param limit	The limit of number of instructions to execute that was set with 
 * 		the -l option 
 *
 ********************************************************************************/
void rv32i::run(uint64_t limit)
{
	uint64_t i = 0;
	uint32_t memory_amount = mem->get_size();			//get the size
	regs.set(2, memory_amount);					//store it in register 2

	while(!halt && (limit == 0 || i++ < limit))		        //while halt is false or less than limit size 
	{
		tick();							//call tick function
		
	}
	
	//if halt is true, that means we encountered an ebreak instruction, so print line 	
	if(halt == true)
	{
		std::cout << "Execution terminated by EBREAK instruction\n";
	}
	//print out total number of instructions executed 
	std::cout << std::dec << insn_counter << " instructions executed" << std::endl; 
	
}
/**
 * Accessor for halt member variable.
 *
 * @return halt	 The bool variable that determines if we are to stop executing
 *
 ********************************************************************************/
bool rv32i::is_halted() const
{
	return halt;
}
/**
 * Executes a given RV32I instruction
 *
 * This dcex() function makes use of a nested switch statement to help execute.
 * First it makes use of the get_opcode, get_funct7, get_funct3, get_special_funct,
 * and get_imm_i to set variables.  It then begins by evaluating the opcode and 
 * depending on the opcode, it will then look to the value of the funct3 or funct7 
 * to disntinguish what instruction it is. 
 *
 * @param insn	The instruction that will get executed
 * ********************************************************************************/

void rv32i::dcex(uint32_t insn, std::ostream* pos)
{
	uint32_t opcode = get_opcode(insn);
	uint32_t funct3 = get_funct3(insn);
	uint32_t funct7 = get_funct7(insn);
	uint32_t special_funct7 = get_special_funct7(insn);

	switch (opcode)						//evaluate the opcode of instruction
	{
	default:
		exec_illegal_insn(insn,pos); return;
	
	case opcode_lui:
		exec_lui(insn, pos);return;
		
	case opcode_auipc:
		exec_auipc(insn, pos); return;
		
	case opcode_jal:
		exec_jal(insn, pos); return;
	
	case opcode_jalr:
		exec_jalr(insn,pos); return;
		
	case opcode_fence:
		exec_fence(insn, pos); return;
	case opcode_ecall:
		//if it has the ecall opcode, check the funct7 value to distinguish
		//between ecall and ebreak
		switch(special_funct7)
		{
		default:
			exec_illegal_insn(insn, pos); return;
		case funct7_ecall:
			exec_ecall(insn, pos); return;	
		case funct7_ebreak:
			exec_ebreak(insn, pos); return;
		}
		assert(0 && "unhandled funct7");
	
	case opcode_rtype:
		switch(funct3)					//we know it's r type, compare funct3 to see what kind of r instruction
		{
		default:
			exec_illegal_insn(insn, pos); return;
		//if it has the funct3_add value
		case funct3_add:
			//check the funct7 value to distinguish between add and sub
			switch(funct7)
			{
			default:
				exec_illegal_insn(insn, pos); return;
			case funct7_add:
				exec_add(insn, pos); return;
			case funct7_sub:
				exec_sub(insn, pos); return;
			}
			assert(0 && "unhandled funct7");
		case funct3_sll:
			exec_sll(insn, pos); return;
		case funct3_slt:
			exec_slt(insn, pos); return;
		case funct3_sltu:
			exec_sltu(insn, pos); return;
		case funct3_xor:
			exec_xor(insn, pos); return;
		//if it has the funct3_srl value
		case funct3_srl:
			//check the funct7 value to distinguish between srl and sra
			switch(funct7)
			{
			default:
				exec_illegal_insn(insn, pos); return;
			case funct7_srl:
				exec_srl(insn, pos); return;
			case funct7_sra:
				exec_sra(insn, pos); return;
			}
			assert(0 && "unhandled funct7");
		case funct3_or:
			exec_or(insn, pos); return;
		case funct3_and:
			exec_and(insn, pos); return;
		}
		assert(0 && "unhandled funct3");
	
	case opcode_load_imm:
		switch(funct3)
		{
		default:
			exec_illegal_insn(insn,pos); return;
		case funct3_lb:
			exec_lb(insn, pos); return;
		case funct3_lh:
			exec_lh(insn, pos); return;
		case funct3_lw:
			exec_lw(insn, pos); return;
		case funct3_lbu:
			exec_lbu(insn, pos); return;
		case funct3_lhu:
			exec_lhu(insn, pos); return;
		}
		assert(0 && "unhandled funct3");
	
	case opcode_itype:
		switch(funct3)
		{
		default:
			exec_illegal_insn(insn, pos); return;
		case funct3_addi:
			exec_addi(insn, pos); return;
		case funct3_slti:
			exec_slti(insn, pos); return;
		case funct3_sltiu:
			exec_sltiu(insn, pos); return;
		case funct3_xori:
			exec_xori(insn, pos); return;
		case funct3_ori:
			exec_ori(insn, pos); return;
		case funct3_andi:
			exec_andi(insn, pos); return;
		case funct3_slli:
			exec_slli(insn, pos); return;
		//if it has the funct3 srli value
		case funct3_srli:
			//check the funct7 value to distinguish between srli and srai
			switch(funct7)
			{
			default:
				exec_illegal_insn(insn, pos); return;
			case funct7_srli:
				exec_srli(insn, pos); return;
			case funct7_srai:
				exec_srai(insn, pos); return;
			}
			assert(0 && "unhandled funct7");
		}
		assert(0 && "unhandled funct3");
	
	case opcode_btype:
		switch(funct3)
		{
		default:
			exec_illegal_insn(insn,pos); return;	
		case funct3_beq:
		       	exec_beq(insn, pos); return;
	        case funct3_bne:
		       	exec_bne(insn, pos); return;
		case funct3_blt:
		       	exec_blt(insn, pos); return;
	 	case funct3_bge:
			exec_bge(insn, pos); return;
		case funct3_bltu:
			exec_bltu(insn, pos); return;
		case funct3_bgeu:
			exec_bgeu(insn, pos); return;
		}
		assert(0 && "unhandled funct3");
	
	case opcode_stype:
		switch(funct3)
		{
		default:
			exec_illegal_insn(insn, pos); return;
		case funct3_sb:
			exec_sb(insn, pos); return;
		case funct3_sh:
			exec_sh(insn, pos); return;
		case funct3_sw:
			exec_sw(insn, pos); return;
		}		
		assert(0 && "unhandled funct3");
	
	}
	assert(0 && "unhandled opcode");
	
}
/**
 * Decodes an instruction and returns the text that contains the rendering of 
 * the instruction.
 *
 * This decode() function makes use of a nested switch statement to help decode.
 * First it makes use of the get_opcode, get_funct7, get_funct3, get_special_funct,
 * and get_imm_i to set variables.  It then begins by evaluating the opcode and 
 * depending on the opcode, it will then look to the value of the funct3 or funct7 
 * to disntinguish what instruction it is. 
 *
 * @param insn	The instruction obtained from disasm function
 * 		that will get decoded.
 *
 * @return An std::string that contains the instruction in text.
 *
 ********************************************************************************/
std::string rv32i::decode(uint32_t insn) const
{
	//the following 5 lines get the various fields of the insn to compare in the 
	//switch statement to identify the instruction	
	uint32_t opcode = get_opcode(insn);
	uint32_t funct3 = get_funct3(insn);
	uint32_t funct7 = get_funct7(insn);
	uint32_t special_funct7 = get_special_funct7(insn);
	int32_t imm_i = get_imm_i(insn);

	switch (opcode)						//evaluate the opcode of instruction
	{
	default:
		return render_illegal_insn();
	case opcode_lui:
		return render_lui(insn);
	case opcode_auipc:
		return render_auipc(insn);
	case opcode_jalr:
		return render_jalr(insn);
	case opcode_jal:
		return render_jal(insn);
	case opcode_fence:
		return render_fence(insn);
	case opcode_ecall:
		//if it has the ecall opcode, check the funct7 value to distinguish
		//between ecall and ebreak
		switch(special_funct7)
		{
		default:
			return render_illegal_insn();
		case funct7_ecall:
			return render_ecall(insn);
		case funct7_ebreak:
			return render_ebreak(insn);
		}
		assert(0 && "unhandled funct7");
	case opcode_rtype:
		switch(funct3)					//we know it's r type, compare funct3 to see what kind of r instruction
		{
		default:
			return render_illegal_insn();
		//if it has the funct3_add value
		case funct3_add:
			//check the funct7 value to distinguish between add and sub
			switch(funct7)
			{
			default:
				return render_illegal_insn();
			case funct7_add:
				return render_rtype(insn, "add ");
			case funct7_sub:
				return render_rtype(insn, "sub ");
			}
			assert(0 && "unhandled funct7");
		case funct3_sll:
			return render_rtype(insn, "sll ");
		case funct3_slt:
			return render_rtype(insn, "slt ");
		case funct3_sltu:
			return render_rtype(insn, "sltu ");
		case funct3_xor:
			return render_rtype(insn, "xor ");
		//if it has the funct3_srl value
		case funct3_srl:
			//check the funct7 value to distinguish between srl and sra
			switch(funct7)
			{
			default:
				return render_illegal_insn();
			case funct7_srl:
				return render_rtype(insn, "srl ");
			case funct7_sra:
				return render_rtype(insn, "sra ");
			}
			assert(0 && "unhandled funct7");
		case funct3_or:
			return render_rtype(insn, "or ");
		case funct3_and:
			return render_rtype(insn, "and ");
		}
		assert(0 && "unhandled funct3");
		
	case opcode_load_imm:
		switch(funct3)
		{
		default:
			return render_illegal_insn();
		case funct3_lb:
			return render_itype_load(insn, "lb ");
		case funct3_lh:
			return render_itype_load(insn, "lh ");
		case funct3_lw:
			return render_itype_load(insn, "lw ");
		case funct3_lbu:
			return render_itype_load(insn, "lbu ");
		case funct3_lhu:
			return render_itype_load(insn, "lhu ");
		}
		assert(0 && "unhandled funct3");
	case opcode_itype:
		switch(funct3)
		{
		default:
			return render_illegal_insn();
		case funct3_addi:
			return render_itype_alu(insn, "addi ", imm_i);
		case funct3_slti:
			return render_itype_alu(insn, "slti ", imm_i);
		case funct3_sltiu:
			return render_itype_alu(insn, "sltiu ", imm_i);
		case funct3_xori:
			return render_itype_alu(insn, "xori ", imm_i);
		case funct3_ori:
			return render_itype_alu(insn, "ori ", imm_i);
		case funct3_andi:
			return render_itype_alu(insn, "andi ", imm_i);
		case funct3_slli:
			return render_itype_alu_shamt(insn, "slli ", imm_i % XLEN);
		//if it has the funct3 srli value
		case funct3_srli:
			//check the funct7 value to distinguish between srli and srai
			switch(funct7)
			{
			default:
				return render_illegal_insn();
			case funct7_srli:
				return render_itype_alu_shamt(insn, "srli ", imm_i % XLEN);
			case funct7_srai:
				return render_itype_alu_shamt(insn, "srai ", imm_i % XLEN);
			}
			assert(0 && "unhandled funct7");
		}
		assert(0 && "unhandled funct3");
	case opcode_btype:
		switch(funct3)
		{
		default:
			return render_illegal_insn();
		case funct3_beq:
		       	return render_btype(insn, "beq ");
	        case funct3_bne:
		       	return render_btype(insn, "bne ");
		case funct3_blt:
		       	return render_btype(insn, "blt ");
	 	case funct3_bge:
			return render_btype(insn, "bge ");
		case funct3_bltu:
			return render_btype(insn, "bltu ");
		case funct3_bgeu:
			return render_btype(insn, "bgeu ");
		}
		assert(0 && "unhandled funct3");
	case opcode_stype:
		switch(funct3)
		{
		default:
			return render_illegal_insn();
		case funct3_sb:
			return render_stype(insn, "sb ");
		case funct3_sh:
			return render_stype(insn, "sh ");
		case funct3_sw:
			return render_stype(insn, "sw ");
		}		
		assert(0 && "unhandled funct3");
	
	}
	assert(0 && "unhandled opcode");
	
}
