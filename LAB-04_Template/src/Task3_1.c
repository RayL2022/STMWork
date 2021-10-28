/*
 * Task3_1.c
 *
 */


#include "init.h"

//Result global variables
int result1;
volatile int32_t result2;
volatile int32_t result3;
volatile int32_t result4;

//Operands variables
volatile int32_t num1, num2, num3, num4, num5, x;

void do_task1(void);
void do_task2(void);
void do_task3(void);
void do_task4(void);

// Main Execution Loop
int main(void)
{
	//Initialize the system
	Sys_Init();
	printf("\033[2J"); fflush(stdout);
	printf("\033[0;0H"); fflush(stdout);
	do_task1();
	do_task2();
	do_task3();
	do_task4();


	while(1){
	}
}

void do_task1(void){
	//Task 3 - Task Set 1 - #1
	asm("LDR r2, =0x00000016"); //24 in reg 2
	asm("LDR r3, =0x00000026"); // 26 in reg 3
	asm("ADD r4, r2, r3"); //Add 24 + 26, store in reg 4

	asm("STR r4,%0" : "=m" (result1)); //Store result of addition in result1
	printf("Result 1: %d\r\n", result1); //Should be 60
}

void do_task2(void){
	//Task 3 - Task Set 1 - #2
	result2 = 0;
	num1 = 11;
	num2 = 11;

	asm volatile("MUL %[result], %[oper1], %[oper2]"
		: [result] "+r" (result2) //Read and Written to register, result2
		: [oper1] "r" (num1), [oper2] "r" (num2)); //Only read from num1, num2
	printf("Result 2: %ld\r\n", result2); //Result2 should be 121
}

void do_task3(void){
	//Task 3 - Task Set 1 - #3
	result3 = 0;
	num1 = 2;
	num2 = 3;
	num3 = 5;
	x = 12;

	//Multiplying 2 with x (x = 12)
	asm volatile ("MUL %[result], %[oper1], %[oper2]"
			: [result] "+r" (result3)
			: [oper1] "r" (x), [oper2] "r" (num1));

	//Dividing past result by 3 (=8)
	asm volatile ("SDIV %[result], %[oper1], %[oper2]"
			: [result] "+r" (result3)
			: [oper1] "r" (result3), [oper2] "r" (num2));

	//Adding 5 to Result (=13)
	asm volatile ("ADD %[result], %[oper1], %[oper2]"
			: [result] "+r" (result3)
			: [oper1] "r" (result3), [oper2] "r" (num3));
	printf("Result 3: %ld\r\n", result3); //Printing, total should be 13
}

void do_task4(void){
	//Task 3 - Task Set 1 - #4
	result4 = 0;
	num1 = 2;
	num2 = 3;
	num3 = 5;
	num4 = 1;
	x=12;

	//Dividing x by 3
	asm volatile ("SDIV %[result], %[oper1], %[oper2]"
			: [result] "+r" (result4)
			: [oper1] "r" (x), [oper2] "r" (num2));

	//Multiply result4 (=4) by num1 (=2), and add num3 to that value (=5)
	asm volatile ("MLA %[result], %[oper1], %[oper2], %[oper3]"
			: [result] "+r" (result4)
			: [oper1] "r" (result4), [oper2] "r" (num1), [oper3] "r" (num3));
	printf("Result 4: %ld\r\n", result4); //Printing, total should be 13, as shown in #3
}
