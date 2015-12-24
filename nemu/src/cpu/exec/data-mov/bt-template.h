#include "cpu/exec/template-start.h"

#define instr bt

static void do_execute() {
	cpu.CF = (op_src->val >> op_dest->val) & 1;
	print_asm_template2();
}

make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
