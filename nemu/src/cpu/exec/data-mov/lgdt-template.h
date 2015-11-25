#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute() {
	printf("ldgt\n");
	cpu.eip ++ ;
	cpu.GDTR.base = op_src->val >> 8;
	cpu.GDTR.limit = op_src->val & 0xff;
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
