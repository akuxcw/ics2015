#include "common.h"
#include "burst.h"
#include "rand.h"
#include "misc.h"

#define COL_WIDTH 12
#define LINE_WIDTH 6
#define FLAG_WIDTH (32 - COL_WIDTH)

#include "tlb.h"

tlb_t tlb;

uint32_t rand(int);
hwaddr_t page_read(lnaddr_t);

void init_tlb() {
	memset(tlb.valid, 0, sizeof tlb.valid);
}

void tlb_set_read(lnaddr_t addr, void *data) {
	tlb_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t flag = temp.flag;
	uint32_t line, line_ = 0;
	bool full = true, find = false;
	for(line = 0; line < NR_LINE; ++ line) {
		if(tlb.valid[line]) {
			if(tlb.flag[line] == flag) {
				memcpy(data, tlb.data[line] + col, BURST_LEN);
				find = true;
				break;
			}
		} else {
			full = false;
			line_ = line;
		}
	}
	if(!find) {
		if(full) line_ = rand(addr) & LINE_MASK;
		tlb.valid[line_] = true;
		tlb.flag[line_] = flag;
		int i;
		for(i = 0; i < NR_COL; ++ i) { 
			tlb.data[line_][i] = page_read((addr & ~COL_MASK) + i * 4);
		}
		memcpy(data, tlb.data[line_] + col, BURST_LEN);
	}
}

hwaddr_t tlb_read(lnaddr_t addr) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint32_t len = 4;

	tlb_set_read(addr, temp);

	if(offset + len > BURST_LEN) {
		tlb_set_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	
	return unalign_rw(temp + offset, 4);
}
