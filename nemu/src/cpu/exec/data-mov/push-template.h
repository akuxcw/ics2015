#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	swaddr_write(cpu.esp, 4, op_src->val);
	print_asm_template1();
}

make_instr_helper(r)


#include "cpu/exec/template-end.h"
