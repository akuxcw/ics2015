#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"

uint32_t hwaddr_read(lnaddr_t, size_t);

lnaddr_t seg_translate(swaddr_t addr, uint8_t sreg) {
	uint8_t tmp[8]; 
	int i;
	for(i = 0; i < 8; ++ i) 
		tmp[i] = hwaddr_read(cpu.GDTR.base + cpu.sr[sreg].index * 8 + i, 1);
	SegDesc *segdesc = (SegDesc*)tmp;
//	printf("%s %x\n", sregs[sreg], cpu.sr[sreg].index);
//		printf("%x\n", cpu.GDTR.base );
//	printf("%x\n", 
//		(segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + segdesc->base_15_0 );
	//if(sreg != R_CS) 
	Assert(segdesc->present == 1, "Segdesc is not valid!");
	return 
		(segdesc->base_31_24 << 24) + (segdesc->base_23_16 << 16) + 
		segdesc->base_15_0 + addr;
}

void init_seg() {
	cpu.cs = 0;
/*	SegDecs *segdesc;
	segdesc->present = 1;
	segdesc->base_31_24 = segdesc->base_23_16 = segdesc->base_15_0 = 0;
	segdesc->limit_19_16 = segdesc->limit_15_0 = 0;
	uint8_t tmp[8];
	tmp = (uint8_t *)segdesc;
	for(i = 0; i < 8; ++ i)
		swaddr_write()
*/}
