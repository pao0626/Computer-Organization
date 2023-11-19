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
"addi t1, x0, 8\n\t"                    //for loop limit 8
"add t3, %[p_x], x0\n\t"                //t3=&x[0]
"vsetvli t0, t1, e16, ta, ma\n\t"       //設定 vl，short int（e16）元素
"vle16.v v1, (%[p_x])\n\t"              //Get x array vector
"vadd.vx v2, v2, x0\n\t"                //Get zero vector
"vadd.vx v3, v3, %[target]\n\t"         //v3 = target

"loop3:\n\t"                            //loop
"lh t5, 0(t3)\n\t"                      //t5 = x[i]
"vadd.vx v4, v2, t5\n\t"                //vector[8]={t5}
"addi t3, t3, 2\n\t"                    //i++
"vadd.vv v5, v1, v4\n\t"                //Sum vectors to v5

"vmseq.vv v0, v3, v5\n\t"               //set v4[i] where v5=v3
"vfirst.m t6, v0\n\t"                   //find first set bit
"bgez t6, find\n\t"                     //found

"addi t1, t1, -1\n\t"                   //loop limit -1
"bne t1, zero, loop3\n\t"               //for loop
"j exit\n\t"                            //jump end
"find:\n\t"                             //find target
"addi %[flag], x0, 1\n\t"               //flag=1
"exit:\n\t"                             //end