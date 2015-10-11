#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute() {
	cpu.esp = cpu.ebp;
	DATA_TYPE result = swaddr_read(cpu.esp, DATA_BYTE);
	cpu.esp += DATA_BYTE;
	OPERAND_W(op_src, result); 
	print_asm_template1();
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
