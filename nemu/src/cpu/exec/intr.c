#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"
#include <setjmp.h>
extern jmp_buf jbuf;

uint32_t lnaddr_read(lnaddr_t, size_t);
void load_sreg(uint32_t);
int tyu = 0;

void raise_intr(uint8_t NO) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 *	 * That is, use ``NO'' to index the IDT.
	 *		 */

//	printf("%x\n", jbuf);
	uint8_t tmp[8];
	int i;
	printf("0x%x\n", cpu.IDTR.base + NO * 0x8);
	for(i = 0; i < 8; ++ i) tmp[i] = lnaddr_read(cpu.IDTR.base + NO * 0x8 + i, 1);
	for(i = 0; i < 8; ++ i) printf("%x\n", tmp[i]);
	GateDesc *gde = (GateDesc*)tmp;
	cpu.ss = gde->segment;
	printf("%x\n", gde->segment);
	cpu.eip = (gde->offset_31_16 << 16) + gde->offset_15_0;
	printf("0x%x\n", cpu.eip);
	load_sreg(R_SS);
	/* Jump back to cpu_exec() */
	tyu = 1;
	longjmp(jbuf, 1);
}
