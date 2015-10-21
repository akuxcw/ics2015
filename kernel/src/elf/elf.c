#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>
//#include <stdio.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

void ramdisk_write(uint8_t *, uint32_t, uint32_t);

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;
	ph = (void*)(buf + elf->e_ehsize);


	/* TODO: fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);
//	nemu_assert(elf->e_ehsize == 52);
//	nemu_assert(buf[50] == 0x5);
//	nemu_assert(ph->p_type == PT_LOAD);

//	HIT_GOOD_TRAP;
	int cnt = 0;
	/* Load each program segment */
	for(; true; ) {
		/* Scan the program header table, load each segment into memory */
		if(ph->p_type == PT_LOAD) {
			cnt ++;
			int i;
			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			for(i = 0; i < ph->p_memsz; i ++) 
				ramdisk_write(buf + ph->p_offset + i, ph->p_vaddr + i, 1);
			 
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
//			uint8_t zero = 0;
//			for(i = ph->p_filesz; i < ph->p_memsz; i ++) 
//				ramdisk_write(&zero, 0x800000 + i, 1);
			ph += 4 * elf->e_phentsize;
#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
	//		break;
		} else break;
	}

	nemu_assert(cnt == 2);
	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
