#include "cpu/exec/template-start.h"

extern int len;

#define _exec() cpu.eip += op_src->val;\
			    snprintf(op_src->str, OP_STR_SIZE, "$0x%x", cpu.eip + len + 1)

#define instr ja

static void do_execute() {
	if(cpu.CF == 0 && cpu.ZF == 0) {
		//cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jae

static void do_execute() {
	if(cpu.CF == 0) {
		//cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jb

static void do_execute() {
	if(cpu.CF == 1) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr je

static void do_execute() {
	if(cpu.ZF == 1) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jbe

static void do_execute() {
	if(cpu.ZF == 1 || cpu.CF == 1) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jl

static void do_execute() {
	if(cpu.SF != cpu.OF) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr


#define instr jle

static void do_execute() {
	if(cpu.ZF == 1 || cpu.SF != cpu.OF) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jg

static void do_execute() {
	if(cpu.ZF == 0 && cpu.SF == cpu.OF) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jge

static void do_execute() {
	if(cpu.SF == cpu.OF) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jne

static void do_execute() {
	if(cpu.ZF == 0) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jns

static void do_execute() {
	if(cpu.SF == 0) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr js

static void do_execute() {
	if(cpu.SF == 1) {
//		cpu.eip += op_src->val;
		_exec();
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#include "cpu/exec/template-end.h"
