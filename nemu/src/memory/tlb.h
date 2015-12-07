typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t flag	: FLAG_WIDTH;
	};
	uint32_t addr;
} tlb_addr;

#define NR_COL (1 << (COL_WIDTH - 2))
#define NR_LINE (1 << LINE_WIDTH)
#define NR_FLAG (1 << FLAG_WIDTH)

#define LINE_MASK (NR_LINE - 1)
#define COL_MASK (NR_COL - 1)

typedef	struct {
	hwaddr_t data[NR_LINE][NR_COL];
	bool valid[NR_LINE];
	uint32_t flag[NR_LINE];
} tlb_t;

