#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute() {
//	OPERAND_W(op_dest, op_src->val);
	cpu.edi = cpu.eax;
	print_asm_template2();
}

//make_instr_helper(r2rm)
make_instr_helper(n)

#include "cpu/exec/template-end.h"
