#include "trap.h"

int gcd(int a,int b){
	return b == 0 ? a : gcd(b, a%b);
}

int a[] = {1,2,3,4,5,6,7,8,9,10};
int b[] = {101,102,103,104,105,106,107,108,109,110};
int c[] = {1,2,1,4,5,2,1,4,1,10};
	
int main() {
	int i;
	for(i=0;i<10;++i)
	nemu_assert(gcd(a[i], b[i]) == c[i]);

	HIT_GOOD_TRAP;
	return 0;
}
