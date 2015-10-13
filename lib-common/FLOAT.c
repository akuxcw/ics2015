#include "FLOAT.h"

#define int64_t long long

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	return (FLOAT)(((int64_t)a * (int64_t)b) >> 16) ;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
//	int l = 0, r = 0xffffffff;

	return 0;
}

FLOAT f2F(float a) {
	int i, uf, m, e, s, ans;
	uf = *(int*)&a;
	m = uf & ((1 << 23) - 1);
	e = ((uf >> 23) & ((1 << 8) - 1)) - 127;
	s = uf >> 31;
	ans = 1;
	for(i = 1; i <= e; ++ i) {
		ans = (ans << 1) + ((m & (1 << 22)) >> 22);
		if (ans < 0) return 0x80000000u;
		m = m << 1;
	}
	return (FLOAT)(ans);
}

FLOAT Fabs(FLOAT a) {
	nemu_assert(0);
	return 0;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

