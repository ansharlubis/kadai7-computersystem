#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void alu32_init(ALU32 *alu32, Path *ops, Bus *a, Bus *b, Bus *s){
	int i;
	
	Path *c;
	c = (Path *)malloc(sizeof(Path) * 30);

	alu32 -> alus = (ALU *)malloc(sizeof(ALU) * 31);
	alu32 -> alumsb = (ALU_MSB *)malloc(sizeof(ALU_MSB));

	Path *set, *null;
	set = (Path *)malloc(sizeof(Path));
	null = (Path *)malloc(sizeof(Path));

	path_init(set); path_init(null);

	alu_init(alu32 -> alus, ops, ops+1, ops+2, a -> p, b -> p, set, ops+2, s -> p, c);			// set is still 0
	for(i = 1; i < 31; i++){
		alu_init(alu32 -> alus+i, ops, ops+1, ops+2, a -> p+i, b -> p+i, null, c+i-1, s -> p+i, c+i);
	}
	alu_msb_init(alu32 -> alumsb, ops, ops+1, ops+2, a -> p+31, b -> p+31, null, c+30, s -> p+31, null, set);

}

void alu32_run(ALU32 *alu32){
	int i;
	for(i = 0; i < 31; i++){
		alu_run(alu32 -> alus+i);
	}
	alu_msb_run(alu32 -> alumsb);
	
}

void alu32_release(ALU32 *alu32){
	free(alu32 -> alumsb -> f1 -> xorc2 -> ogate1 -> out1);		// free null
	free(alu32 -> alumsb -> f1 -> ogate1 -> out1);				// free set

	int i;
	for(i = 0; i < 31; i++){
		alu_release(alu32 -> alus+i);			
	}
	alu_msb_release(alu32 -> alumsb);

	free(alu32 -> alus);
	free(alu32 -> alumsb);
}

void alu32_driver(int na, int nb, Signal o1, Signal o2, Signal bin){
	ALU32 alu32;
	Bus a, b, s;
	Path ops;

	bus_init(&a, 32); bus_init(&b, 32); bus_init(&s, 32);
	bus_set_value(&a, na); bus_set_value(&b, nb);

	path_init(&ops);
	path_set_signal(&ops, o1); path_set_signal(&ops+1, o2); path_set_signal(&ops+2, bin);

	alu32_init(&alu32, &ops, &a, &b, &s);

	alu32_run(&alu32);
	printf("ALU32(op: %d, %d)(bin: %d)(a: %d, b: %d) => %d\n", o2, o1, bin, na, nb, bus_get_value(&s));
	alu32_release(&alu32);

}