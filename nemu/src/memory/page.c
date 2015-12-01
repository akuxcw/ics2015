#include "common.h"
#include "../../lib-common/x86-inc/mmu.h"
#include "cpu/reg.h"

typedef union {
	struct {
		uint32_t offset	:	12;
		uint32_t page	:	10;
		uint32_t dir	:	10;
	};
	uint32_t val;
} lnaddr_st;

uint32_t hwaddr_read(lnaddr_t, size_t);

hwaddr_t page_translate(lnaddr_t addr) {
	printf("***\n");
	lnaddr_st lnaddr;
	lnaddr.val = addr;
	
	printf("0x%x\n", cpu.cr3.val);//page_directory_base);
	PDE dir_entry;
	dir_entry.val = hwaddr_read(cpu.cr3.page_directory_base + 4 * lnaddr.dir, 4);

	printf("0x%x\n", dir_entry.page_frame);
	PTE pg_tbl_entry;
	pg_tbl_entry.val = hwaddr_read((dir_entry.page_frame << 0) + 4 * lnaddr.page, 4);

	printf("0x%x\n", pg_tbl_entry.page_frame);
	return hwaddr_read((pg_tbl_entry.page_frame << 0) + lnaddr.offset, 4);
}
