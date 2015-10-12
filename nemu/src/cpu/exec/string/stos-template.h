#include "cpu/exec/template-start.h"

#define instr stos

static void do_execute() {
	op_src->type = op_dest->type = OP_TYPE_REG;
	op_src->reg = R_ESI; op_dest->reg = R_EDI;
	swaddr_write(cpu.edi, DATA_BYTE, (DATA_TYPE) cpu.eax);
	cpu.edi += cpu.DF == 0? +DATA_BYTE : -DATA_BYTE;
	print_asm_template2();
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
