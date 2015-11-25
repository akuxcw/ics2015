#include "common.h"
#include "memory/mmu.h"
#include "cpu/reg.h"

uint32_t lnaddr_read(lnaddr_t, size_t);

lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg) {
	if(addr < 0x100006) return addr;
	uint32_t tmp[2]; 
	tmp[0] = lnaddr_read(cpu.GDTR.base + cpu.sr[sreg].index * 8 + 8, 4);
	tmp[1] = lnaddr_read(cpu.GDTR.base + cpu.sr[sreg].index * 8 + 12, 4);
	SegDesc *segdesc = (SegDesc*)tmp;
//	printf("%x %x\n", cpu.GDTR.base )
	printf("%x %x %x\n", segdesc->base_31_24, segdesc->base_23_16, segdesc->base_15_0);
	return 
		(segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + 
		segdesc->base_15_0 + addr;
}
