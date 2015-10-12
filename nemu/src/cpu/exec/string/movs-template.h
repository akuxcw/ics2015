#include "cpu/exec/template-start.h"

#define instr movs

static void do_execute() {
//	OPERAND_W(op_dest, op_src->val);
	swaddr_write(cpu.edi, DATA_BYTE, swaddr_read(cpu.esi, DATA_BYTE));
	cpu.edi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	cpu.esi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	print_asm_template2();
}

//make_instr_helper(r2rm)
make_instr_helper(n)

#include "cpu/exec/template-end.h"
