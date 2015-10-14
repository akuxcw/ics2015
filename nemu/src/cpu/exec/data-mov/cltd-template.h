#include "cpu/exec/template-start.h"

#define instr cltd

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	if((cpu.eax >> 31) != 0) cpu.edx = 0xffffffff; else cpu.edx = 0x0;
	print_asm("cltd");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
