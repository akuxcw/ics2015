#include "FLOAT.h"
//#include <asm/div64.h>
#define int64_t long long

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	int a1 = (a >> 16),b1 = (b >> 16);
	int a0 = a & 0xffff,b0 = b & 0xffff;
	int c0,c1,c2,c3;
	c0 = a0 * b0;
	c1 = c0 / 0xffff + a0 * b1 + a1 * b0; c0 %= 0xffff;
	c2 = c1 / 0xffff + a1 * b1; c1 %= 0xffff;
	c3 = c2 / 0xffff; c2 %= 0xffff;
	int ans = c1 + (c2 << 16);
	return ans;
//	return (FLOAT)(((int64_t)a * (int64_t)b) >> 16) ;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
//	int l = 0, r = 0xffffffff;
//	return (FLOAT)((int64_t)a / b);
	int64_t c = (int64_t)a << 16ll;
//	do_div(c,b);
	return c / b;
	//return ((int64_t)a << 16ll) / b;
}

FLOAT f2F(float a) {
	int i, uf, m, e, s, ans;
	uf = *(int*)&a;
	m = uf & ((1 << 23) - 1);
	e = ((uf >> 23) & ((1 << 8) - 1)) - 127;
	s = uf >> 31;
	ans = 1;
	for(i = 1; i <= e + 16; ++ i) {
		ans = (ans << 1) + ((m & (1 << 22)) >> 22);
		if (ans < 0) return 0x80000000u;
		m = m << 1;
	}
	if (s != 0) ans = (~ans) + 1;
	return (FLOAT)(ans);
}

FLOAT Fabs(FLOAT a) {
	return (a >> 31) == 0 ? a : (~a) + 1;
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

