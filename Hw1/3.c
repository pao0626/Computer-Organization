#include<stdio.h>
int main() {
	int f,i=0;
	int h[9]={0}, x[3]={0}, y[3]={0};
	FILE *input = fopen("../input/3.txt","r");
	for(i = 0; i<9; i++) fscanf(input, "%d", &h[i]);
	for(i = 0; i<3; i++) fscanf(input, "%d", &x[i]);
	for(i = 0; i<3; i++) fscanf(input, "%d", &y[i]);
	fclose(input);
	int *p_x = &x[0] ;
	int *p_h = &h[0] ;
	int *p_y = &y[0] ;

/* Original C code segment
    for (f = 0 ; f < 3; f++)
        *p_y += *p_h++ * *p_x++ ;
*/
	for (i = 0 ; i < 3; i++)
    {
    	p_x = &x[0]  ;
    	for (f = 0 ; f < 3; f++)
    		asm volatile(
    			"lw t0, 0(%[H])\n\t"
    			"lw t1, 0(%[X])\n\t"
    			"lw t2, 0(%[Y])\n\t"
    			"mul t3, t0, t1\n\t"
    			"add t2, t2, t3\n\t"
    			"sw t2, 0(%[Y])\n\t"
    			"addi %[H], %[H], 4\n\t"
    			"addi %[X], %[X], 4\n\t"
    			:[H] "+r"(p_h)
  			,[X] "+r"(p_x)
  			,[Y] "+r"(p_y)
        	);
        p_y++;
	}
	p_y = &y[0];
	for(i = 0; i<3; i++)
		printf("%d \n", *p_y++);
	return(0)  ;
}
