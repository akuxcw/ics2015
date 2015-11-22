#include "common.h"
#include "burst.h"
#include "rand.h"
#include "misc.h"

#define COL_WIDTH 6
#define LINE_WIDTH 3
#define SET_WIDTH 7
#define FLAG_WIDTH (27 - COL_WIDTH - SET_WIDTH)

#include "cache.h"

cache_set cache[NR_SET];

uint32_t rand(int);
uint32_t dram_read(hwaddr_t, size_t);
uint32_t L2_cache_read(hwaddr_t, size_t);

void init_cache() {
	memset(cache, 0, sizeof cache);
}

void cache_set_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	uint32_t line, line_ = 0;
	bool full = true, find = false;
	for(line = 0; line < NR_LINE; ++ line) {
		if(cache[set].valid[line]) {
			if(cache[set].flag[line] == flag) {
				memcpy(data, cache[set].data[line] + col, BURST_LEN);
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
		cache[set].valid[line_] = true;
		cache[set].flag[line_] = flag;
		int i;
		for(i = 0; i < NR_COL; ++ i) { 
			cache[set].data[line_][i] = L2_cache_read((addr & ~COL_MASK) + i, 1);
		}
		memcpy(data, cache[set].data[line_] + col, BURST_LEN);
	}
}

void cache_set_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	
	uint32_t line;
	for(line = 0; line < NR_LINE; ++ line) {
		if(cache[set].valid[line] && cache[set].flag[line] == flag) {
			memcpy_with_mask(cache[set].data[line] + col, data, BURST_LEN, mask);
			break;
		}
	}
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	cache_set_read(addr, temp);

	if(offset + len > BURST_LEN) {
		cache_set_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	
//	printf("%d", success);
	return unalign_rw(temp + offset, 4);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];
	uint8_t mask[2 * BURST_LEN];
	memset(mask, 0, 2 * BURST_LEN);

	*(uint32_t *)(temp + offset) = data;
	memset(mask + offset, 1, len);

	cache_set_write(addr, temp, mask);

	if(offset + len > BURST_LEN) {
		cache_set_write(addr + BURST_LEN, temp + BURST_LEN, mask + BURST_LEN);
	}
}
