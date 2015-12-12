#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"
#include <setjmp.h>
extern jmp_buf jbuf;

uint32_t lnaddr_read(lnaddr_t, size_t);
void load_sreg(uint32_t);
int tyu = 0;

void raise_intr(uint32_t NO) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 *	 * That is, use ``NO'' to index the IDT.
	 *		 */

//	printf("%x\n", jbuf);
	uint8_t tmp[8];
	int i;
	for(i = 0; i < 8; ++ i) lnaddr_read(cpu.IDTR.base + NO * 0x8 + i, 1);
	printf("0x%x\n", cpu.eip);
	GateDesc *gde = (GateDesc*)tmp;
	cpu.ss = gde->segment;
	load_sreg(R_SS);
	cpu.eip = (gde->offset_31_16 << 16) + gde->offset_15_0;
	/* Jump back to cpu_exec() */
	tyu = 1;
	longjmp(jbuf, 1);
}
