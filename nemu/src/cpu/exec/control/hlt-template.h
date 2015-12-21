#include "cpu/exec/template-start.h"

#define instr hlt

//void raise_intr(uint8_t, uint32_t);

static void do_execute() {
//	cpu.eip --;
	while(!(cpu.INTR & cpu.IF));
	print_asm("hlt");
}

make_instr_helper(n)

#include "cpu/exec/template-end.h"
