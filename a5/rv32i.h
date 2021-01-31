
//***************************************************************************
//
//  rv32i.h
//  CSCI 463 Assignment 5
//
//  Created by Jennifer Paul (z1878099)
//
//***************************************************************************
#ifndef rv32i_h
#define rv32i_h
#include "registerfile.h"
#include <stdint.h>
#include "memory.h"
#include <string>
class rv32i
{
	public:
		rv32i(memory *m);
		void disasm(void);
		std::string decode(uint32_t insn) const;   
		void set_show_instructions(bool b);
		void set_show_registers(bool b);
		bool is_halted() const;
		void reset();
		void dump() const;
		void dcex(uint32_t insn, std::ostream*);
		void exec_illegal_insn(uint32_t insn, std::ostream* pos);
		void exec_ebreak(uint32_t insn , std::ostream* pos);
		void exec_fence(uint32_t insn, std::ostream* pos);
		void exec_ecall(uint32_t insn, std::ostream* pos);
		void exec_lui(uint32_t insn, std::ostream* pos);
		void exec_auipc(uint32_t insn, std::ostream* pos);
		void exec_jal(uint32_t insn, std::ostream* pod);
		void exec_jalr(uint32_t insn, std::ostream* pos);
		void exec_bne(uint32_t insn, std::ostream* pos);
		void exec_blt(uint32_t insn, std::ostream* pos);
		void exec_bge(uint32_t insn, std::ostream* pos);
		void exec_bltu(uint32_t insn, std::ostream* pos);
		void exec_bgeu(uint32_t insn, std::ostream* pos);
		void exec_beq(uint32_t insn, std::ostream* pos);
		void exec_slt(uint32_t insn, std::ostream* pos);
		void exec_sltu(uint32_t insn, std::ostream* pos);
		void exec_addi(uint32_t insn, std::ostream* pos);
		void exec_lbu(uint32_t insn, std::ostream* pos);
		void exec_lhu(uint32_t insn, std::ostream* pos);
		void exec_lb(uint32_t insn, std::ostream* pos);
		void exec_lh(uint32_t insn, std::ostream* pos);
		void exec_lw(uint32_t insn, std::ostream* pos);
		void exec_sb(uint32_t insn, std::ostream* pos);
		void exec_sh(uint32_t insn, std::ostream* pos);
		void exec_sw(uint32_t insn, std::ostream* pos);
		void exec_slti(uint32_t insn, std::ostream* pos);
		void exec_sltiu(uint32_t insn, std::ostream* pos);
		void exec_xori(uint32_t insn, std::ostream* pos);
		void exec_ori(uint32_t insn, std::ostream* pos);
		void exec_andi(uint32_t insn, std::ostream* pos);
		void exec_slli(uint32_t insn, std::ostream* pos);
		void exec_srli(uint32_t insn, std::ostream* pos);
		void exec_srai(uint32_t insn, std::ostream* pos);
		void exec_add(uint32_t insn, std::ostream* pos);
		void exec_sub(uint32_t insn, std::ostream* pos);
		void exec_sll(uint32_t insn, std::ostream* pos);
		void exec_srl(uint32_t insn, std::ostream* pos);
		void exec_sra(uint32_t insn, std::ostream* pos);
		void exec_xor(uint32_t insn, std::ostream* pos);
		void exec_or(uint32_t insn, std::ostream* pos);
		void exec_and(uint32_t insn, std::ostream* pos);
		void tick();
		void run(uint64_t limit);
		static uint32_t get_opcode(uint32_t insn);
		static uint32_t get_rd(uint32_t insn);
		static uint32_t get_funct3(uint32_t insn);
		static uint32_t get_rs1(uint32_t insn);
		static uint32_t get_rs2(uint32_t insn);
		static uint32_t get_funct7(uint32_t insn);
		static uint32_t get_special_funct7(uint32_t insn);
		static uint32_t get_fence_funct7(uint32_t insn);
		static uint32_t get_pred(uint32_t insn);
		static uint32_t get_succ(uint32_t insn);
		static int32_t get_imm_i(uint32_t insn);
		static int32_t get_imm_u(uint32_t insn);
	        static int32_t get_imm_b(uint32_t insn);
		static int32_t get_imm_s(uint32_t insn);
		static int32_t get_imm_j(uint32_t insn);
		std::string render_illegal_insn() const;
		std::string render_lui(uint32_t insn) const;
		std::string render_auipc(uint32_t insn) const;
		std::string render_rtype(uint32_t insn, const char *mnemonic) const;
		std::string render_itype_load(uint32_t insn, const char *mnemonic) const;
		std::string render_itype_alu_shamt(uint32_t insn, const char *mnemonic, int32_t imm_i) const;
		std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i) const;
		std::string render_btype(uint32_t insn, const char *mnemonic) const;
		std::string render_stype(uint32_t insn, const char *mnemonic) const;
		std::string render_jal(uint32_t insn) const;
		std::string render_jalr(uint32_t insn) const;
		std::string render_ecall(uint32_t insn) const;
		std::string render_ebreak(uint32_t insn) const;
		std::string render_fence(uint32_t insn) const;

 	private:
		registerfile regs;
		bool halt = false;
		bool show_instructions = false;
		bool show_registers = false;
		uint64_t insn_counter;
		memory *mem;
		uint32_t pc = 0;
		static constexpr int mnemonic_width = 8;
		static constexpr int instruction_width = 35;
		static constexpr uint32_t XLEN = 32;
		static constexpr uint32_t opcode_fence = 0b0001111;
		static constexpr uint32_t opcode_lui = 0b0110111;
		static constexpr uint32_t opcode_auipc = 0b0010111;
		static constexpr uint32_t opcode_ecall = 0b1110011;
		static constexpr uint32_t opcode_btype = 0b1100011;
		static constexpr uint32_t opcode_rtype = 0b0110011;
		static constexpr uint32_t opcode_stype = 0b0100011;
		static constexpr uint32_t opcode_load_imm = 0b0000011;
		static constexpr uint32_t opcode_itype = 0b0010011;
		static constexpr uint32_t opcode_jal = 0b1101111;
		static constexpr uint32_t opcode_jalr = 0b1100111;
		static constexpr uint32_t funct7_ecall= 0b000000000000;
		static constexpr uint32_t funct7_ebreak= 0b000000000001;
		static constexpr uint32_t funct7_fence = 0b0000;
		static constexpr uint32_t funct7_add = 0b0000000;
		static constexpr uint32_t funct7_sub = 0b0100000;
		static constexpr uint32_t funct7_srl = 0b0000000;
		static constexpr uint32_t funct7_sra = 0b0100000;
		static constexpr uint32_t funct7_srli = 0b0000000;
		static constexpr uint32_t funct7_srai = 0b0100000;
		static constexpr uint32_t funct3_add = 0b000;
		static constexpr uint32_t funct3_sll = 0b001;
		static constexpr uint32_t funct3_slt = 0b010;
		static constexpr uint32_t funct3_sltu = 0b011;
		static constexpr uint32_t funct3_xor = 0b100;
		static constexpr uint32_t funct3_srl = 0b101;
		static constexpr uint32_t funct3_or = 0b110;
		static constexpr uint32_t funct3_and = 0b111;
		static constexpr uint32_t funct3_lb = 0b000;
		static constexpr uint32_t funct3_lh = 0b001;
		static constexpr uint32_t funct3_lw = 0b010;
		static constexpr uint32_t funct3_lbu = 0b100;
		static constexpr uint32_t funct3_lhu = 0b101;
		static constexpr uint32_t funct3_addi = 0b000;
		static constexpr uint32_t funct3_slti = 0b010;
		static constexpr uint32_t funct3_sltiu = 0b011;
		static constexpr uint32_t funct3_xori = 0b100;
		static constexpr uint32_t funct3_ori = 0b110;
		static constexpr uint32_t funct3_andi = 0b111;
		static constexpr uint32_t funct3_slli = 0b001;
		static constexpr uint32_t funct3_srli = 0b101;
		static constexpr uint32_t funct3_beq = 0b000;
		static constexpr uint32_t funct3_bne = 0b001;
		static constexpr uint32_t funct3_blt = 0b100;
		static constexpr uint32_t funct3_bge = 0b101;
		static constexpr uint32_t funct3_bltu = 0b110;
		static constexpr uint32_t funct3_bgeu = 0b111;
		static constexpr uint32_t funct3_sb = 0b000;
		static constexpr uint32_t funct3_sh = 0b001;
		static constexpr uint32_t funct3_sw = 0b010;
		static constexpr int One = 1;
		static constexpr int Two = 2;
		static constexpr int Three = 3;
		static constexpr int Four = 4;
		static constexpr int Five = 5;
		static constexpr int Six = 6;
		static constexpr int Seven = 7;
		static constexpr int Eight = 8;
		static constexpr int Nine = 9;
		static constexpr int Ten = 10;
		static constexpr int Eleven = 11;
		static constexpr int Twelve = 12;
		static constexpr int Thirteen = 13;
		static constexpr int Fourteen = 14;
		static constexpr int Fifteen = 15;
};
#endif


