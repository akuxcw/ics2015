#include "trap.h"

int gcd(int a,int b){
	return b == 0 ? a : gcd(b, a%b);
}

int main() {
	int a = 100;
	int b = 10;
	nemu_assert(gcd(a, b) == 10);

	HIT_GOOD_TRAP;
	return 0;
}
