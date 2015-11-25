#include "common.h"
#include "memory/mmu.h"
#include "cpu/reg.h"

uint32_t lnaddr_read(lnaddr_t, size_t);

lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg) {
	uint32_t tmp[2]; 
	tmp[0] = lnaddr_read(cpu.GDTR.base + cpu.sr[sreg].index * 8, 4);
	tmp[1] = lnaddr_read(cpu.GDTR.base + cpu.sr[sreg].index * 8 + 4, 4);
	SegDesc *segdesc = (SegDesc*)tmp;
	return (segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0 + addr;
}