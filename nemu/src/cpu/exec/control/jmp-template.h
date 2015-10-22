#include "cpu/exec/template-start.h"

#define instr jmp

extern int len;

static void do_execute() {
//	printf("%x %d\n", op_src->val, DATA_BYTE);
	if(DATA_BYTE == 4 && op_src->val>0x100000 && MSB(op_src->val) == 0) {
//		printf("%x\n", op_src->val);
		op_src->val -= cpu.eip + len + 1;
	}
	cpu.eip += op_src->val;
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
