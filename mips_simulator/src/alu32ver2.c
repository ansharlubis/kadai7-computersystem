#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void alu32ver2_init(ALU32 *alu32, Path *op1, Path *op2, Path *binvert, Bus *a, Bus *b, Bus *s, Path *carryout){
	int i;

	Path *c;
	c = (Path *)malloc(sizeof(Path) * 31);

	for(i = 0; i < 31; i++){
		path_init(c+i);
	}

	alu32 -> alus = (ALU *)malloc(sizeof(ALU) * 32);

	alu_init(alu32 -> alus, op1, op2, binvert, a -> p, b -> p, binvert, s -> p, c);
	for(i = 1; i < 31; i++){
		alu_init(alu32 -> alus+i, op1, op2, binvert, a -> p+i, b -> p+i, c+i-1, s -> p+i, c+i);
	}
	alu_init(alu32 -> alus+31, op1, op2, binvert, a -> p+31, b -> p+31, c+30, s -> p+31, carryout);

}

void alu32ver2_run(ALU32 *alu32){
	int i;
	for(i = 0; i < 32; i++){
		alu_run(alu32 -> alus+i);
	}
}

void alu32ver2_release(ALU32 *alu32){
	free(alu32 -> alus -> f1 -> ogate1 -> out1);

	int i;
	for(i = 0; i < 32; i++){
		alu_release(alu32 -> alus+i);
	}

	free(alu32 -> alus);
}

void alu32ver2_driver(int na, int nb, Signal o1, Signal o2, Signal bin){
	ALU32 alu32;
	Bus a, b, s;
	Path op1, op2, binvert;
	Path carryout;

	bus_init(&a, 32); bus_init(&b, 32); bus_init(&s, 32);
	bus_set_value(&a, na); bus_set_value(&b, nb);

	path_init(&op1); path_init(&op2); path_init(&binvert);
	path_set_signal(&op1, o1); path_set_signal(&op2, o2); path_set_signal(&binvert, bin);
	path_init(&carryout);

	alu32ver2_init(&alu32, &op1, &op2, &binvert, &a, &b, &s, &carryout);

	alu32ver2_run(&alu32);
	printf("ALU32(op: %d, %d)(bin: %d)(a: %d, b: %d) => %d\n", o2, o1, bin, na, nb, bus_get_value(&s));
	alu32ver2_release(&alu32);
}