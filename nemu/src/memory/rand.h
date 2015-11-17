#ifndef __RAND_H__
#define __RAND_H__


uint32_t rand(int addr) {
	return ((addr % 10007) * (addr % 1007)) % 100007;
}

#endif
