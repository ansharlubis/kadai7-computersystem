#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void alu32_init(ALU32 *alu32, Path *op2, Path *op1, Bus *a, Bus *b, Path *bin, Path *carryin, Bus *s, Path *carryout){
	int i;
	
	Bus *c;
	c = (Bus *)malloc(sizeof(Bus));
	bus_init(c, 31);

	alu32 -> alus = (ALU *)malloc(sizeof(ALU) * 32);

	alu_init(alu32 -> alus, op2, op1, a -> p, b -> p, bin, carryin, s -> p, carryout);
	for(i = 1; i < 30; i++){
		alu_init(alu32 -> alus+i, op2, op1, a -> p+i, b -> p+i, bin, c -> p+i-1, s -> p+i, c -> p+i);	
	}
	alu_init(alu32 -> alus+31, op2, op1, a -> p+31, b -> p+31, bin, c -> p+30, s -> p+31, carryout);
}

void alu32_run(ALU32 *alu32){
	int i;
	for(i = 0; i < 32; i++){
		alu_run(alu32 -> alus+i);
	}
}

void alu32_release(ALU32 *alu32){
	int i;
	free((alu32 -> alus+1) -> f1 -> agate2 -> in1);

	for(i = 0; i < 32; i++){
		alu_release(alu32 -> alus+i);
	}

	free(alu32 -> alus);
}