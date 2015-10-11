#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	if(op_src->val>0x10000) {
		op_src->val -= cpu.eip;//swaddr_read(op_src->val, 4);
//		printf("%x %x\n",op_src-> )
	}
	cpu.eip += op_src->val;
//	printf("%x\n",cpu.eip);
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
