#include "cpu/exec/template-start.h"

#define instr je

static void do_execute() {
	if(cpu.ZF == 1) {
		cpu.eip += op_src->val;
	}
	print_asm_template1();
}

make_instr_helper(i)

#undef instr

#define instr jbe

static void do_execute() {
	if(cpu.ZF == 1 || cpu.CF == 1) {
		printf("%x\n",op_src->val);
		cpu.eip += op_src->val < 0x80 ? op_src->val : -((~op_src->val)+1);
	}
	print_asm_template1();
}

make_instr_helper(i)

#undef instr

#include "cpu/exec/template-end.h"
