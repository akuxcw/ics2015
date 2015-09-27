#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	op_dest->reg = R_ESP;
	strcpy(op_dest->str ,REG_NAME(R_ESP));
	op_dest->val = REG(R_ESP);
//	OPERAND_W(op_dest, op_dest->val - op_src->val);
	printf("***%d %d\n",R_ESP,cpu.esp);
	REG(R_ESP) = REG(R_ESP) - op_src->val;
	printf("***%d %d\n",R_ESP,cpu.esp);
	print_asm_template2();
}

//make_instr_helper(i2r)
make_instr_helper(i2rm)
//make_instr_helper(r2rm)
//make_instr_helper(rm2r)

/*
make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}
*/
#include "cpu/exec/template-end.h"
