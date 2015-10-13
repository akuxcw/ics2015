#include "trap.h"
#include "FLOAT.h"

int main() {

	FLOAT x = f2F(0.233);
	FLOAT y = sqrt(x);
	y = F_mul_F(y, y);
	nemu_assert(Fabs(x - y) < f2F(1e-4));

	HIT_GOOD_TRAP;

	return 0;
}
