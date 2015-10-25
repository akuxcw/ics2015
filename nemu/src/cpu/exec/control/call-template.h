#include "cpu/exec/template-start.h"

#define instr call

extern int len;

static void do_execute() {
	cpu.esp -= 4;
	if(op_src->val < 0x100000 || 0xffffffff - op_src->val < 0x100000) {
		swaddr_write(cpu.esp, 4, cpu.eip + len);
		cpu.eip += op_src->val;
		op_src->val = cpu.eip + len + 1;
	}else {
		swaddr_write(cpu.esp, 4, cpu.eip + len);
		cpu.eip = op_src->val - 2;
	}
	print_asm_template1();
}

make_instr_helper(si)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
