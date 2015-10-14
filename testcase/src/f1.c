#include "trap.h"
#include "FLOAT.h"

int main() {

	FLOAT x = f2F(0.233);
	FLOAT y = f2F(0.23301);
	nemu_assert(Fabs(x - y) < f2F(1e-3));

	HIT_GOOD_TRAP;

	return 0;
}
