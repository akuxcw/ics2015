#define MAX_LEVEL II

#define COL_WIDTH 6
#define LINE_WIDTH 3
#define SET_WIDTH 7
#define FLAG_WIDTH (27 - COL_WIDTH - SET_WIDTH)
#define WROTE_BACK
#define LEVEL I
#include "cache-template.h"
#include "cache-undef.h"


#define COL_WIDTH 6
#define LINE_WIDTH 4
#define SET_WIDTH 12
#define FLAG_WIDTH (27 - COL_WIDTH - SET_WIDTH)
#define WRITE_THOUGH
#define LEVEL II
#include "cache-template.h"
#include "cache-undef.h"

void init_cache() {
	init_cache_I();
}

uint32_t cache_read(hwaddr_t addr, size_t len) {
	return cache_read_I(addr, len);
}

void cache_write(hwaddr_t addr, size_t len, uint32_t data) {
	cache_write_I(addr, len, data);
}
