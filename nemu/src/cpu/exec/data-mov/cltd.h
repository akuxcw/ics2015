#ifndef __CLTD_H__
#define __CLTD_H__

make_helper(cltd) {
	if((cpu.eax >> 31) == 1) cpu.edx = 0xffffffff; else cpu.edx = 0x0;
	cpu.eip ++;
	print_asm("cltd");
	return 0;
}

#endif
