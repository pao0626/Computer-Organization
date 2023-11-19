/* original C code
	for (int i = 0; i < arr_size; i++){
		p_y[i] = p_h[i] + p_x[i];
	}
*/

// TODO : 
"li t1, 16\n\t"				//array size 16
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"loop2:\n\t"//loop
"addi %[others_cnt], %[others_cnt], 1\n\t"	
"vsetvli t0, t1, e16, ta, ma\n\t" // 設定 vl(一次處理t0個元素)，每個元素 short int（e16）(SEW)
"addi %[others_cnt], %[others_cnt], 1\n\t"
"vle16.v v0, (%[h])\n\t"    //Get first vector
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"sub t1, t1, t0\n\t" //Decrement number done
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"slli t0, t0, 1\n\t" //Multiply number done by 2 bytes
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"add %[h], %[h], t0\n\t" 
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"vle16.v v1, (%[x])\n\t"    //Get second vector
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"add %[x], %[x], t0\n\t"
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"vadd.vv v2, v0, v1\n\t"    //Sum vectors
"addi %[arith_cnt], %[arith_cnt], 1\n\t"	
"vse16.v v2, (%[y])\n\t"    //store res
"addi %[sw_cnt], %[sw_cnt], 1\n\t"
"add %[y], %[y], t0\n\t"
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"bne t1, zero, loop2\n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"	
// :"t0", "t1", "v0", "v1", "v2" 

