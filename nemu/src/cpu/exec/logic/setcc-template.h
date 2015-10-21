#include "cpu/exec/template-start.h"
/*
#define instr ja

static void do_execute() {
	if(cpu.CF == 0 && cpu.ZF == 0) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

*/

#define instr sete

static void do_execute() {
	if(cpu.ZF == 1) {
		 OPERAND_W(op_src, 1);
	}else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr

	/*
#define instr jbe

static void do_execute() {
	if(cpu.ZF == 1 || cpu.CF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jl

static void do_execute() {
	if(cpu.SF != cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr


#define instr jle

static void do_execute() {
	if(cpu.ZF == 1 || cpu.SF != cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jg

static void do_execute() {
	if(cpu.ZF == 0 && cpu.SF == cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr

#define instr jge

static void do_execute() {
	if(cpu.SF == cpu.OF) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(si)

#undef instr
*/
#define instr setne

static void do_execute() {
	if(cpu.ZF == 0) {
		 OPERAND_W(op_src, 1);
	}else OPERAND_W(op_src, 0);
	print_asm_template1();
}

make_instr_helper(rm)

#undef instr

#include "cpu/exec/template-end.h"
