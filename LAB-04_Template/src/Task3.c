//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
//
//

#include "init.h"

int result1;
// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();

	//Task 3 - Task Set 2 - #1
	asm("LDR r2, =0x00000018"); //24 in reg 2
	asm("LDR r3, =0x0000001A"); // 26 in reg 3
	asm("ADD r4, r2, r3"); //Add 24 + 26, store in reg 4

	asm("STR r4,%0" : "=m" (result1));
	printf("%d", result1); fflush(stdout);

	//Task 3 - Task Set 2 - #2
	float num1, num2, result2;
	num1 = 0.5;
	num2 = 1.5;

	asm("VMUL.F32 %[result], %[oper1], %[oper2]"
		: [result] "+t" (result)
		: [oper1] "t" (num1), [oper2] "t" (num2));




	while(1){
	}
}
