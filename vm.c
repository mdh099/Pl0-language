/*
	Jacob Cordonero
  Mathew Holden
  Gabriella On-Cuen
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define REG_FILE_SIZE 10
#define MAX_STACK_LENGTH 100

typedef struct IR{
  int opCode; // operation code
  int r; // register
  int l; // lexigraphical level
  int m; // Depending on the operation code: A number / program or data address
} IR; 

void print_execution(int line, char *opname, IR IR, int PC, int BP, int SP, int *stack, int *RF)
{
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t\t", line, opname, IR.r, IR.l, IR.m, PC, SP, BP);
	
	// print register file
	for (i = 0; i < REG_FILE_SIZE; i++)
		printf("%d ", RF[i]);
	printf("\n");
	
	// print stackPC
	printf("stack:\t");
	for (i = MAX_STACK_LENGTH - 1; i >= SP; i--)
		printf("%d ", stack[i]);
	printf("\n");
}

int base(int L, int BP, int *stack)
{
	int ctr = L;
	int rtn = BP;
	while (ctr > 0)
	{
		rtn = stack[rtn];
		ctr--;
	}
	return rtn;
}

// Fetch function is called while(halt != true), 
// it is pased the instruction code, and the address
// to our IR struct in the execute program.
void fetch_cycle(instruction code, IR *IR){
    IR->opCode = code.opcode; 
    IR->r = code.r; 
    IR->l = code.l;
    IR->m = code.m;
}

// Here lies the execute cycle. 
// This is where the instruction placed in the IR are executed using the data and register stacks.
// Function is passed an instruction struct, which has opCode, R, L, M
void execute_program(instruction *code, int printFlag)
{
  bool halt = false; 

  int RF[REG_FILE_SIZE] = {0}; // Make register
  int stack[MAX_STACK_LENGTH] = {0}; //Make stack

  // Initial values
  int BP = MAX_STACK_LENGTH - 1;
  int SP = BP + 1;
  int PC = 0; 
  IR IR; // Creates the IR struct that we will be filling each fetch cycle
  
  int line = 0;
  char *opName; // this keeps track of the opname to print out

	// keep this
	if (printFlag)
	{
		printf("\t\t\t\t\tPC\tSP\tBP\n");
		printf("Initial values:\t\t\t\t%d\t%d\t%d\n", PC, SP, BP);
	}

	while(halt != true)
	{
     line = PC; //the line number to print
    // instruction pointed to by program counter (PC) is fetched and placed in       
    // instruction register 
    // Code is a pointer, pc is used to iterate through the operation codes.
    // Calls fetch cycle, provides the instruction in code[PC],
    // and provides the address to the IR struct that get filled with the code instructions.
    fetch_cycle(code[PC], &IR); 
    PC++;   //Iterate to next code
	  
    int error = 0; // create error flag
    
		// Execute Cycle
    switch(IR.opCode){
      case 1: 
        // LIT instruction: sets value at register IR.r to IR.m
	      opName = "LIT";
        RF[IR.r] = IR.m;
        break;
      
      case 2: 
        // RET instruction: Return from current procedure (X) to the last procedure (Y).
        opName = "RET";
        SP = BP + 1; // SP = index of the end of Y's AR (BP + 1)
        BP = stack[SP-2]; // BP = dynamic link value from X's AR
        PC = stack[SP-3]; // PC = return address value from X's AR 
        break;
      
      case 3: 
        // LOD instruction: Load value to register IR.R from the stack location at offset RF[IR.M] from L lexicographical levels up 
	      opName = "LOD";
        //Access Error by checking if base(L) - M is less than zero or greater than or equal to max stack length
        if ((base(IR.l, BP, stack) - RF[IR.m]) < 0 || (base(IR.l, BP, stack) - RF[IR.m]) >= MAX_STACK_LENGTH) {
        halt = true;
        error = 2; 
        break;
        }
        RF[IR.r] = stack[base(IR.l, BP, stack) - RF[IR.m]]; 
        break;
        
      case 4: 
        // STO instruction: Store value from register IR.R to the stack location at offset RF[IR.M] from L lexicographical levels down 
		    opName = "STO";
        //Access Error by checking if base(L) - M is less than zero or greater than or equal to max stack length
        if ((base(IR.l, BP, stack) - RF[IR.m]) < 0 || (base(IR.l, BP, stack) - RF[IR.m]) >= MAX_STACK_LENGTH) {
          halt = true;
          error = 2; 
          break;
        }
        stack[base(IR.l, BP, stack) - RF[IR.m]] = RF[IR.r];
        break;
      
      case 5: 
        // CAL instruction: Call procedure at code index M. This will generate a new Activation Record. 
        opName = "CAL";
        stack[SP-1] = base(IR.l, BP, stack); // 1st - static link = base(L)
        stack[SP-2] = BP; // 2nd - dynamic link = BP
        stack[SP-3] = PC; // 3rd - return address = PC
        // After creating the activation record,
        BP = SP - 1; // BP = the index of the first entry of the new AR 
        PC = IR.m; 
        break;
      
      case 6: 
        // INC instruction: Decrement SP by M, check for Stack Overflow Error which can occur if SP < 0 after the decrement 
        opName = "INC";
        SP -= IR.m;
        if(SP<0) {
          halt = true;  
          error = 1;   
          break;
        }
        break;
        
      case 7: 
        // JMP instruction: Jump to instruction M
		    opName = "JMP";
        PC = IR.m;
        break;
        
      case 8: 
        // JPC instruction: Jump to instruction M if register R is 
	    	opName = "JPC";
        if(RF[IR.r]==0){
          PC = IR.m;
        }
        break;
        
      case 9: 
        // WRT instruction: Print register R
	      opName = "WRT";
        printf("Write Value: %d\n", RF[IR.r]);
        break;
        
      case 10: 
        // RED instruction: Register R equals scanf() 
		    opName = "RED";
        printf("Please Enter a Value: ");
        scanf("%d", &RF[IR.r]);
        printf("\n");
        break;
        
      case 11: 
        // HAL instruction: End of program (Set Halt flag to true) 
		    opName = "HLT";
        halt = true;
        break;
        
      case 12: 
        // NEG instruction: Negate the register R 
		    opName = "NEG";
        RF[IR.r] = (-1*RF[IR.r]);
        break;
        
      case 13: 
        // ADD instruction: Add the registers L and M and store the result in register R 
		    opName = "ADD";
        RF[IR.r] = RF[IR.l] + RF[IR.m]; 
        break;
        
      case 14: 
        // SUB instruction: Subtract register M from register L and store the result in register R 
		    opName = "SUB";
        RF[IR.r] = RF[IR.l] - RF[IR.m];
        break;
        
      case 15: 
        // MUL instruction: Multiply registers L and M and store the result in register R 
		    opName = "MUL";
        RF[IR.r] = RF[IR.l] * RF[IR.m];
        break;
        
      case 16: 
        // DIV instruction: Divide register L by register M and store the result in register R 
		    opName = "DIV";
        RF[IR.r] = RF[IR.l] / RF[IR.m];
        break;
        
      case 17: 
        // MOD instruction: Set register R equal to register L modulo register M 
		    opName = "MOD";
        RF[IR.r] = RF[IR.l] % RF[IR.m];
        break;
        
      case 18: 
        // EQL instruction: If register L equals register M, set register R to 1. Otherwise set register R to 0 
		    opName = "EQL";
        if (RF[IR.l] == RF[IR.r]){
          RF[IR.r] = 1;
        }
        else{
          RF[IR.r] = 0;
        }
        break;
        
      case 19: 
        // NEQ instruction: If register L does not equal register M, set register R to 1. Otherwise set register R to 0 
		    opName = "NEQ";
         if (RF[IR.l] != RF[IR.m]){
          RF[IR.r] = 1;
        }
        else{
          RF[IR.r] = 0;
        }
        break;
        
      case 20: 
        // LSS instruction: If register L is less than register M, set register R to 1. Otherwise set register R to 0 
		    opName = "LSS";
        if(RF[IR.r]<RF[IR.m]){
          RF[IR.r] = 1;
        }
        else{
          RF[IR.r] = 0;
        }
        break;
        
      case 21: 
        // LEQ instruction: If register L is less than or equal to register M, set register R to 1. Otherwise set register R to 0 
		    opName = "LEQ";
        if(RF[IR.r]<=RF[IR.m]){
          RF[IR.r] = 1;
        }
        else{
          RF[IR.r] = 0;
        }
        break;
        
      case 22: 
        // GTR instruction: If register L is greater than register M, set register R to 1. Otherwise set register R to 0 
		    opName = "GTR";
        if(RF[IR.r]>RF[IR.m]){
          RF[IR.r] = 1;
        }
        else{
          RF[IR.r] = 0;
        }
        break;
        
      case 23: 
        // GEQ instruction: If register L is greater than or equal to register M, set register R to 1. Otherwise set register R to 0 
		    opName = "GEQ";
        if(RF[IR.r]>=RF[IR.m]){
          RF[IR.r] = 1;
        }
        else{
          RF[IR.r] = 0;
        }
        break;
    }
    
    switch(error) {//print trace or error message
      case 1:
        printf("Virtual Machine Error: Stack Overflow Error\n");
        break;
      case 2:
        printf("Virtual Machine Error: Out of Bounds Access Error\n");
        break;
      default:
        print_execution(line, opName, IR, PC, BP, SP, stack, RF); //only print when there are no errors
    }   
	}
}
