#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	op_dest->reg = R_ESP;
	op_dest->str[0]='%';op_dest->str[1]='e';
	op_dest->str[2]='s';op_dest->str[3]='p';
	op_dest->str[4]='\0';
	op_dest->val = REG(R_ESP);
	cpu.esp = cpu.esp - op_src->val;
	print_asm_template2();
}

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
