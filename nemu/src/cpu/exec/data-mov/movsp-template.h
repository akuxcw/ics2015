#include "cpu/exec/template-start.h"

#define instr movsp
/*
static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
*/
make_helper(concat(movsp_rm2cr_, SUFFIX)) {
	cpu.CR0 = 1;
	print_asm_template2();
	return 1;
}

make_helper(concat(movsp_cr2rm_, SUFFIX)) {
	print_asm_template2();
	return 1;
}

#include "cpu/exec/template-end.h"
