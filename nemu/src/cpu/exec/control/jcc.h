#ifndef __JCC_H__
#define __JCC_H__
/*
make_helper(mov_i2r_b);
make_helper(mov_i2rm_b);
make_helper(mov_r2rm_b);
make_helper(mov_rm2r_b);
make_helper(mov_a2moffs_b);
make_helper(mov_moffs2a_b);

make_helper(mov_i2r_v);
make_helper(mov_i2rm_v);
make_helper(mov_r2rm_v);
make_helper(mov_rm2r_v);
make_helper(mov_a2moffs_v);
make_helper(mov_moffs2a_v);
*/

make_helper(ja_si_b);

make_helper(je_si_b);

make_helper(jbe_si_b);

make_helper(jl_si_b);

make_helper(jle_si_b);
make_helper(jle_si_v);

make_helper(jg_si_b);

make_helper(jge_si_b);

make_helper(jne_si_b);

#endif

