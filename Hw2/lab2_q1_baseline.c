/* original C code
	for (int i = 0; i < arr_size; i++){
		p_y[i] = p_h[i] + p_x[i];
	}
*/

// TODO : 
"li t0, 0\n\t" 				//for loop i=0
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"li t1, 16\n\t"				//for loop limit 16
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"loop:\n\t"
"addi %[others_cnt], %[others_cnt], 1\n\t"	
"lh t2, 0(%[h])\n\t"		//t2 = p_h[0]
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"lh t3, 0(%[x])\n\t"		//t3 = p_x[0]
"addi %[lw_cnt], %[lw_cnt], 1\n\t"
"add t4, t2, t3\n\t"		//t4 = p_h[0] + p_x[0];
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"sh t4, 0(%[y])\n\t"        //p_y[0] = t2
"addi %[sw_cnt], %[sw_cnt], 1\n\t"
"addi %[h], %[h], 2\n\t"	//p_h++
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"addi %[x], %[x], 2\n\t"	//p_x++
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"addi %[y], %[y], 2\n\t"	//p_y++
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"addi t0, t0, 1\n\t" 		//i++
"addi %[arith_cnt], %[arith_cnt], 1\n\t"
"blt t0, t1, loop\n\t" 	    //if(i<16) go to loop
"addi %[others_cnt], %[others_cnt], 1\n\t"	
// :"t0", "t1", "t2", "t3", "t4" 