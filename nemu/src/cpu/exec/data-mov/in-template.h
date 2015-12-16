#include "cpu/exec/template-start.h"

#define instr in

uint32_t pio_read(ioaddr_t, size_t);

make_helper(concat(in_, SUFFIX)) {
/*	switch(DATA_BYTE) {
		case 1 : reg_b(R_AL) = pio_read(reg_w(R_DX), 1); break;
		case 2 : reg_w(R_AX) = pio_read(reg_w(R_DX), 2); break;
		case 4 : reg_l(R_EAX) = pio_read(reg_w(R_DX), 4); break;
		default : panic("Error in \"in\"!");
	}
*/	
	concat(reg_, SUFFIX)(R_EAX) = pio_read(reg_w(R_DX), DATA_BYTE);
	print_asm("in" " (%%dx),%%%s", concat(regs, SUFFIX)[R_EAX]);
	return 1;
}

#include "cpu/exec/template-end.h"
