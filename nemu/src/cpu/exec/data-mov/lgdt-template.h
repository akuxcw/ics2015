#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute() {
	cpu.GDTR.limit = lnaddr_read(op_src->addr, 2);
	cpu.GDTR.base = lnaddr_read(op_src->addr + 2, 4);
	printf("0x%x 0x%x %s\n", cpu.GDTR.limit, cpu.GDTR.base, op_src->str);
	printf("%x %x %x %x\n", cpu.sr[0].index, cpu.sr[1].index, cpu.sr[2].index, cpu.sr[3].index);
	printf("%x %x %x %x\n", cpu.sr[0]._16, cpu.sr[1]._16, cpu.sr[2]._16, cpu.sr[3]._16);
	printf("%x %x %x %x\n", cpu.es, cpu.cs, cpu.ss, cpu.ds);
	print_asm_template1();	
}

make_instr_helper(rm);

/*
uint32_t read_ModR_M(uint32_t, Operand*, Operand*);
make_helper(concat(lgdt_, SUFFIX)) {
	Operand *rm = op_src;
	Operand *reg = op_dest;
	
	int len = read_ModR_M(eip + 1, rm, reg);
	lnaddr_t addr = instr_fetch(eip + 2, 4);
	cpu.GDTR.limit = lnaddr_read(addr, 2);
	cpu.GDTR.base = lnaddr_read(addr + 2, 4);

	panic("hahaha");	
	cpu.GDTR.limit = lnaddr_read(op_src->val, 2);
	cpu.GDTR.base = lnaddr_read(op_src->val + 2, 4);
	
//	print_asm("lgdt" str(SUFFIX) " 0x%x", addr); 
	print_asm_template1();
	return len + 1;
}
*/
#include "cpu/exec/template-end.h"
