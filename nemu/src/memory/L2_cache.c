#include "common.h"
#include "burst.h"
#include "rand.h"
#include "misc.h"

#define COL_WIDTH 6
#define LINE_WIDTH 4
#define SET_WIDTH 12
#define FLAG_WIDTH (27 - COL_WIDTH - SET_WIDTH)

#include "cache.h"

cache_t L2_cache;

uint32_t rand(int);
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

void init_L2_cache() {
	memset(L2_cache.set, 0, sizeof L2_cache.set);
}

void write_back(uint32_t set, uint32_t line, hwaddr_t addr) {
	if(!L2_cache.set[set].dirty[line]) return;
	uint32_t col;
	for(col = 0; col < NR_COL; ++ col) 
		dram_write((addr & ~COL_MASK) + col, 1, L2_cache.set[set].data[line][col]);
}

void write_L2_cache(uint32_t set, uint32_t line, uint32_t flag, uint32_t addr) {
	L2_cache.set[set].valid[line] = true;
	L2_cache.set[set].dirty[line] = false;
	L2_cache.set[set].flag[line] = flag;
	uint32_t col;
	for(col = 0; col < NR_COL; ++ col) 
		L2_cache.set[set].data[line][col] = dram_read((addr & ~COL_MASK) + col, 1);
}

void L2_cache_set_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	uint32_t line, line_ = 0;
	bool full = true, find = false;
	for(line = 0; line < NR_LINE; ++ line) {
		if(L2_cache.set[set].valid[line]) {
			if(L2_cache.set[set].flag[line] == flag) {
				memcpy(data, L2_cache.set[set].data[line] + col, BURST_LEN);
				find = true;
				break;
			}
		} else {
			full = false;
			line_ = line;
		}
	}
	if(!find) {
		if(full) {
			line_ = rand(addr) & LINE_MASK;
			write_back(set, line_, addr);
		}
		write_L2_cache(set, line_, flag, addr);
		memcpy(data, L2_cache.set[set].data[line_] + col, BURST_LEN);
	}
}

void L2_cache_set_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	uint32_t line, line_ = 0;
	bool full = true, find = false;
	
	for(line = 0; line < NR_LINE; ++ line) {
		if(L2_cache.set[set].valid[line]) {
			if(L2_cache.set[set].flag[line] == flag) {
				find = true;
				L2_cache.set[set].dirty[line] = true;
				memcpy_with_mask(L2_cache.set[set].data[line] + col, data, BURST_LEN, mask);
				break;
			}
		} else {
			full = false;
			line_ = line;
		}
	}
	
	if(!find) {
		if(full) {
			line_ = rand(addr) & LINE_MASK;
			write_back(set, line_, addr);
		}
		write_L2_cache(set, line_, flag, addr);
		memcpy_with_mask(L2_cache.set[set].data[line_] + col, data, BURST_LEN, mask);
	}

}

uint32_t L2_cache_read(hwaddr_t addr, size_t len) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	L2_cache_set_read(addr, temp);

	if(offset + len > BURST_LEN) {
		L2_cache_set_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	
	return unalign_rw(temp + offset, 4);
//	printf("%d", success);
}

void L2_cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	L2_cache_set_write(addr, temp, mask);

	if(offset + len > BURST_LEN) {
		L2_cache_set_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}
