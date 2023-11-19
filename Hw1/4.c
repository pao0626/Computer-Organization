#include<stdio.h>
int main() {
	int i=0;
	int h[9]={0}, x[3]={0}, y[3]={0};
	FILE *input = fopen("../input/4.txt","r");
	for(i = 0; i<9; i++) fscanf(input, "%d", &h[i]);
	for(i = 0; i<3; i++) fscanf(input, "%d", &x[i]);
	for(i = 0; i<3; i++) fscanf(input, "%d", &y[i]);
	fclose(input);
	int *p_x = &x[0] ;
	int *p_h = &h[0] ;
	int *p_y = &y[0] ;
/* Original C code segment
	for (i = 0 ; i < 3; i++){
		p_x = &x[0] ;
    	for (f = 0 ; f < 3; f++)
    		*p_y += *p_h++ * *p_x++ ;
		p_y++ ; 
	}
*/
	asm volatile(
		"li t0, 0\n\t" 				//for loop i=0
		"li t5, 3\n\t"				//for loop limit 3
	"outer_loop:\n\t"
		"add s2, x0, %[X]\n\t"		//s2 = %[X]
		"li t3, 0\n\t" 				//t3 = 0 (sum)
		"li t1, 0\n\t" 				//for loop f=0
	"inner_loop:\n\t"	
		"lw s3, 0(s2)\n\t"			//s3 = *p_x
		"lw s4, 0(%[H])\n\t"		//s4 = *p_h
    	"mul t4, s3, s4\n\t"		//t4 = *p_h * *p_x;
    	"add t3, t3, t4\n\t"		//t3 += t4 (sum)
    	"addi %[H], %[H], 4\n\t"	//p_h++
    	"addi s2, s2, 4\n\t"		//p_x++
    	
		"addi t1, t1, 1\n\t" 		//f+=1	
		"blt t1, t5, inner_loop\n\t" 	//if(f<3) go to inner_loop
		
		"sw t3, 0(%[Y])\n\t"		//*p_y =sum
		"addi %[Y], %[Y], 4\n\t"	//p_y++
		
		"addi t0, t0, 1\n\t" 		//i+=1
		"blt t0, t5, outer_loop\n\t" 	//if(f<3) go to outer_loop
		:[H] "+r"(p_h)
  		,[X] "+r"(p_x)
  		,[Y] "+r"(p_y)	
	);
	p_y = &y[0];
	for(i = 0; i<3; i++)
		printf("%d \n", *p_y++);
	return(0)  ;
}
