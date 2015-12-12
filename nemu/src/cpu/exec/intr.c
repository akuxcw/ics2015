#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"
#include <setjmp.h>
extern jmp_buf jbuf;

uint32_t lnaddr_read(lnaddr_t, size_t);

void raise_intr(uint8_t NO) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	 *	 * That is, use ``NO'' to index the IDT.
	 *		 */

	uint8_t tmp[8];
	int i;
	for(i = 0; i < 8; ++ i) lnaddr_read(cpu.IDTR.base + NO * 0x8 + i, 1);
	GateDesc *gde = (GateDesc*)tmp;
	cpu.ss = gde->segment;
	cpu.eip = (gde->offset_31_16 << 16) + gde->offset_15_0;
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}