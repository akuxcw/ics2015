#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
	DATA_TYPE result = swaddr_read(cpu.esp, DATA_BYTE);
	cpu.esp += DATA_BYTE + op_src->val;
	cpu.eip = result;
	printf("%x\n",op_src->val);
	if(op_src->val == 0) print_asm("ret"); else print_asm_template1();
}

make_instr_helper(n)
make_instr_helper(i)

#include "cpu/exec/template-end.h"
