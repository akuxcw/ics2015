
typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t set	: SET_WIDTH;
		uint32_t flag	: FLAG_WIDTH;
	};
	uint32_t addr;
} cache_addr;

#define NR_COL (1 << COL_WIDTH)
#define NR_LINE (1 << LINE_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_FLAG (1 << FLAG_WIDTH)

#define HW_MEM_SIZE (1 << 27)

#define LINE_MASK (NR_LINE - 1)
#define COL_MASK (NR_COL - 1)

typedef struct {
	struct {
		uint8_t data[NR_LINE][NR_COL];
		bool valid[NR_LINE];
		uint32_t flag[NR_LINE];
	} set[NR_SET];
	void (*read)(hwaddr_t, void *);
	void (*write)(hwaddr_t, void *, uint8_t *);
} cache_t;

