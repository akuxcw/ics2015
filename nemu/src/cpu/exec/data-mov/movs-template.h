#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
//	OPERAND_W(op_dest, op_src->val);
	cpu.edi = cpu.esi;
	print_asm_template2();
}

//make_instr_helper(r2rm)
make_instr_helper(n)

#include "cpu/exec/template-end.h"
