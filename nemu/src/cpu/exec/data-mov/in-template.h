#include "cpu/exec/template-start.h"

#define instr in

uint32_t pio_read(ioaddr_t, size_t);

make_helper(concat(in_, SUFFIX)) {
	reg_b(R_AL) = pio_read(reg_w(R_DX), 1);

	print_asm("in" " (%%dx),%%al");
	return 1;
}

#include "cpu/exec/template-end.h"
