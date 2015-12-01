#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"

uint32_t hwaddr_read(lnaddr_t, size_t);

hwaddr_t page_translate(lnaddr_t addr) {
	printf("***\n");
	return addr - 0xc0000000; 
}
