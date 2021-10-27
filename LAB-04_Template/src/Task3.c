//--------------------------------
// Lab 4 - Sample - Lab04_sample.c
//--------------------------------
//
//

#include "init.h"

//Result global variables
int result1;
volatile float result2;
volatile float result3;
volatile float result4;

//Operands variables
volatile float num1, num2, num3, num4, x;

void do_task1(void);
void do_task2(void);
void do_task3(void);
void do_task4(void);

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();

	do_task1();
	do_task2();
	do_task3();
	do_task4();


	while(1){
	}
}

void do_task1(void){
	//Task 3 - Task Set 2 - #1
	asm("LDR r2, =0x00000018"); //24 in reg 2
	asm("LDR r3, =0x0000001A"); // 26 in reg 3
	asm("ADD r4, r2, r3"); //Add 24 + 26, store in reg 4

	asm("STR r4,%0" : "=m" (result1)); //Store result of addition in result1
	printf("Result 1: %d\r\n", result1); //Should be 50
}

void do_task2(void){
	//Task 3 - Task Set 2 - #2
	result2 = 0;
	num1 = 0.5;
	num2 = 1.5;

	asm volatile("VMUL.F32 %[result], %[oper1], %[oper2]"
		: [result] "+t" (result2) //Read and Written to register, result2
		: [oper1] "t" (num1), [oper2] "t" (num2)); //Only read from num1, num2
	printf("Result 2: %f\r\n", (double)result2); //Result2 should be 0.75
}

void do_task3(void){
	//Task 3 - Task Set 2 - #3
	result3 = 0;
	num1 = 2;
	num2 = 3;
	num3 = 5;
	x = 1.5;

	//Dividing past result by 3
	asm volatile ("VDIV.F32 %[result], %[oper1], %[oper2]"
			: [result] "+t" (result3)
			: [oper1] "t" (num1), [oper2] "t" (num2));

	//Multiplying 2 with x (x = 1.5)
	asm volatile ("VMUL.F32 %[result], %[oper1], %[oper2]"
			: [result] "+t" (result3)
			: [oper1] "t" (result3), [oper2] "t" (x));

	//Adding 5 to Result
	asm volatile ("VADD.F32 %[result], %[oper1], %[oper2]"
			: [result] "+t" (result3)
			: [oper1] "t" (result3), [oper2] "t" (num3));
	printf("Result 3: %f\r\n", (double)result3); //Printing, total should be 6
}

void do_task4(void){
	//Task 3 - Task Set 2 - #4
	result4 = 0;
	num4 = 1;
	//x = 3; //Distinction from #3, should be 7 for result

	//Doesn't change, still need to divide by 3
	asm volatile ("VDIV.F32 %[result], %[oper1], %[oper2]"
			: [result] "+t" (num1)
			: [oper1] "t" (num1), [oper2] "t" (num2));

	//We can now multiply result with x and add to result sum
	asm volatile ("VMLA.F32 %[result], %[oper1], %[oper2]"
			: [result] "+t" (result4)
			: [oper1] "t" (num1), [oper2] "t" (x));

	//Multiply 5 by 1 and Add to result (pure addition to result with MAC)
	asm volatile ("VMLA.F32 %[result], %[oper1], %[oper2]"
			: [result] "+t" (result4)
			: [oper1] "t" (num4), [oper2] "t" (num3));
	printf("Result 4: %f\r\n", (double)result4); //Printing, total should be 6, as shown in #3
}
