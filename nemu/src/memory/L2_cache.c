#include "common.h"
#include "burst.h"
#include "rand.h"
#include "misc.h"

#define COL_WIDTH 6
#define LINE_WIDTH 4
#define SET_WIDTH 12
#define FLAG_WIDTH (27 - COL_WIDTH - SET_WIDTH)

typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t set	: SET_WIDTH;
		uint32_t flag	: FLAG_WIDTH;
	};
	uint32_t addr;
} L2_cache_addr;

#define NR_COL (1 << COL_WIDTH)
#define NR_LINE (1 << LINE_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_FLAG (1 << FLAG_WIDTH)

#define HW_MEM_SIZE (1 << 27)

#define LINE_MASK (NR_LINE - 1)
#define COL_MASK (NR_COL - 1)

typedef struct {
	uint8_t data[NR_LINE][NR_COL];
	bool valid[NR_LINE];
	bool dirty[NR_LINE];
	uint32_t flag[NR_LINE];
} L2_cache_set;

L2_cache_set L2_cache[NR_SET];

uint32_t rand(int);
uint32_t dram_read(hwaddr_t, size_t);

void init_L2_cache() {
	memset(L2_cache, 0, sizeof L2_cache);
}

bool L2_cache_set_read(hwaddr_t addr, void *data) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	L2_cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	uint32_t line, line_ = 0;
	bool full = true, find = false;
	for(line = 0; line < NR_LINE; ++ line) {
		if(L2_cache[set].valid[line]) {
			if(L2_cache[set].flag[line] == flag) {
				memcpy(data, L2_cache[set].data[line] + col, BURST_LEN);
				find = true;
			}
		} else {
			full = false;
			line_ = line;
		}
	}
	if(!find) {
		if(full) line_ = rand(addr) & LINE_MASK;
		L2_cache[set].valid[line_] = true;
		L2_cache[set].flag[line_] = flag;
		int i;
		for(i = 0; i < 64; ++ i) 
			L2_cache[set].data[line_][i] = dram_read((addr & ~COL_MASK) + i, 1);
	}
	return find;
}

void L2_cache_set_write(hwaddr_t addr, void *data, uint8_t *mask) {
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!", addr);

	L2_cache_addr temp;
	temp.addr = addr & ~BURST_MASK;
	uint32_t col = temp.col;
	uint32_t set = temp.set;
	uint32_t flag = temp.flag;
	
	uint32_t line;
	for(line = 0; line < NR_LINE; ++ line) {
		if(L2_cache[set].valid[line] && L2_cache[set].flag[line] == flag) {
			memcpy_with_mask(L2_cache[set].data[line] + col, data, BURST_LEN, mask);
			break;
		}
	}
}

bool L2_cache_read(hwaddr_t addr, size_t len, uint32_t *data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	bool success = L2_cache_set_read(addr, temp);

	if(offset + len > BURST_LEN) {
		success &= L2_cache_set_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	
	*data = unalign_rw(temp + offset, 4);
//	printf("%d", success);
	return success;
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
