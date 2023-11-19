/* original C code 
    for (int i = 0; i < 8; i++){
        if (flag == 1)
            break;

        for (int j = 0; j < 8; j++){
            if ((p_x[i] + p_x[j]) == target){
                flag = 1;
                break;
            }
        }
    }
*/

/* idea code
    for (int i=0; i<8; i++){
        tmp1[8]={p_x[i]};
        用 vadd.vv 相加 p_x 和 tmp1 array，結果存入tmp2 
        查詢tmp2[8]中有沒有符合target結果
        有的話把flag = 1，break
    }
*/

// TODO :
"li t1, 8\n\t"				//for loop limit 8
"li t2, 8\n\t"				//array size 8
"add t3, %[p_x], x0\n\t"    //t3=&x[0]
"loop3:\n\t"                //loop
"vsetvli t0, t2, e16, ta, ma\n\t"   // 設定 vl，short int（e16）元素
"vle16.v v0, (%[p_x])\n\t"    //Get first vector
"vle16.v v1, (%[p_tmp1])\n\t"    //Get second vector
"lh t5, 0(t3)\n\t"      //t5 = x[i]
"vadd.vx v1, v1, t5\n\t"    //tmp1[8]={t5}
"addi t3, t3, 2\n\t"        //i++
"vadd.vv v2, v0, v1\n\t"    //Sum vectors
"vse16.v v2, (%[p_tmp2])\n\t"    //store res

//can be improve
"lh t4, 0(%[p_tmp2])\n\t"
"beq %[target], t4, find\n\t"
"lh t4, 2(%[p_tmp2])\n\t"
"beq %[target], t4, find\n\t"
"lh t4, 4(%[p_tmp2])\n\t"
"beq %[target], t4, find\n\t"
"lh t4, 6(%[p_tmp2])\n\t"
"beq %[target], t4, find\n\t"
"lh t4, 8(%[p_tmp2])\n\t"
"beq %[target], t4, find\n\t"
"lh t4, 10(%[p_tmp2])\n\t"
"beq %[target], t4, find\n\t"
"lh t4, 12(%[p_tmp2])\n\t"
"beq %[target], t4, find\n\t"
"lh t4, 14(%[p_tmp2])\n\t"
"beq %[target], t4, find\n\t"

"addi t1, t1, -1\n\t"       //loop limit -1
"bne t1, zero, loop3\n\t"       //for loop
"j exit\n\t"      //jump end
"find:\n\t"     //find target
"addi %[flag], x0, 1\n\t"   //flag=1
"exit:\n\t"     //end
