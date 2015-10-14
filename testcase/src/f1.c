#include "trap.h"
#include "FLOAT.h"

int main() {

	FLOAT x = f2F(0.24);
	FLOAT y = f2F(0.2);
	FLOAT z = F_div_F(x, y);
	FLOAT a = f2F(1.2);
	nemu_assert(Fabs(z - a) < f2F(1e-4));

	HIT_GOOD_TRAP;

	return 0;
}
