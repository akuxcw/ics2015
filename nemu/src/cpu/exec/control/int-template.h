#include "cpu/exec/template-start.h"

#define instr int

void raise_intr(uint32_t);

static void do_execute() {
	int len = instr_len();
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.EFLAGS, R_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.cs, R_SS);
	cpu.esp -= 4;
	swaddr_write(cpu.esp, 4, cpu.eip + len, R_SS);
	print_asm_template1();
	raise_intr(op_src->val);
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
