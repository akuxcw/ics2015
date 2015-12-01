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
	uint8_t r = instr_fetch(eip + 1, 1);
	printf("0x%x\n", r);
	cpu.cr._[(r >> 3) & 0x7] = REG(r & 0x7);
	printf("*\n");
//	print_asm("movsp" str(SUFFIX) " %%%s,%%%s", REG_NAME(r & 0x7), "cr0");
	printf("**\n");
	return 2;
}

make_helper(concat(movsp_cr2rm_, SUFFIX)) {
	uint8_t r = instr_fetch(eip + 1, 1);
	REG(r & 0x7) = cpu.cr._[(r >> 3) & 0x7];
	print_asm("movsp" str(SUFFIX) " %%%s,%%%s", "cr0", REG_NAME(r & 0x7));
	return 2;
}

#include "cpu/exec/template-end.h"
