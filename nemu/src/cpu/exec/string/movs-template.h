#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
//	OPERAND_W(op_dest, op_src->val);
	swaddr_write(cpu.edi,4,swaddr_read(cpu.esi,4));
	print_asm_template2();
}

//make_instr_helper(r2rm)
make_instr_helper(n)

#include "cpu/exec/template-end.h"
