#include "common.h"
#include "burst.h"
#include "rand.h"

#define COL_WIDTH 6
#define LINE_WIDTH 3
#define SET_WIDTH 7
#define FLAG_WIDTH (27 - COL_WIDTH - LINE_WIDTH - SET_WIDTH)

typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
//		uint32_t line	: LINE_WIDTH;
		uint32_t set	: SET_WIDTH;
		uint32_t flag	: FLAG_WIDTH;
	};
	uint32_t addr;
} cache_addr;

#define NR_COL (1 << COL_WIDTH)
#define NR_LINE (1 << LINE_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_FLAG (1 << FLAG_WIDTH)

#define LINE_MASK (NR_LINE - 1)
#define COL_MASK (NR_COL - 1)

typedef struct {
	uint8_t data[NR_LINE][NR_COL];
	bool valid[NR_LINE];
	uint32_t flag[NR_LINE];
} cache_set;

cache_set cache[NR_SET];

uint32_t rand(int);
uint32_t dram_read(hwaddr_t, size_t);

bool cache_set_read(hwaddr_t addr, void *data) {
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
			}
		} else {
			full = false;
			line_ = line;
		}
	}
	if(!find) {
		if(full) line_ = rand(addr) & LINE_MASK;
		int i;
		for(i = 0; i < 64; ++ i) 
			cache[set].data[line_][i] = dram_read((addr & ~COL_MASK) + i, 1);
	}
	return find;
}

bool cache_read(hwaddr_t addr, size_t len, uint32_t *data) {
	uint32_t offset = addr & BURST_MASK;
	uint8_t temp[2 * BURST_LEN];

	bool success = cache_set_read(addr, temp);

	if(offset + len > BURST_LEN) {
		success &= cache_set_read(addr + BURST_LEN, temp + BURST_LEN);
	}
	
	*data = unalign_rw(temp + offset, 4);
	printf("%d\n", success);
	return success;
}
