#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	if(op_src->val>0x100000)swaddr_read(op_src->val, 4);
	cpu.eip += op_src->val;
//	printf("%x\n",cpu.eip);
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
