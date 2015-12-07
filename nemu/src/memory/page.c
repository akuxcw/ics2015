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
hwaddr_t tlb_read(lnaddr_t);

hwaddr_t page_read(lnaddr_t addr) {
	lnaddr_st lnaddr;
	lnaddr.val = addr;
	
	PDE dir_entry;
	dir_entry.val = 
		hwaddr_read((cpu.cr._3.page_directory_base << 12) + 4 * lnaddr.dir, 4);
	Assert(dir_entry.present == 1, "dir_entry is not valid!");

	PTE pg_tbl_entry;
	pg_tbl_entry.val = hwaddr_read((dir_entry.page_frame << 12) + 4 * lnaddr.page, 4);
	Assert(pg_tbl_entry.present == 1, "pg_tbl_entry is not valid!");

	hwaddr_t hwaddr = (pg_tbl_entry.page_frame << 12) + lnaddr.offset;
	return hwaddr;
}

hwaddr_t page_translate(lnaddr_t addr) {
	return tlb_read(addr);
}
