#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute() {
	DATA_TYPE result = swaddr_read(cpu.esp, DATA_BYTE);
	cpu.eip = result;
	print_asm("ret");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
