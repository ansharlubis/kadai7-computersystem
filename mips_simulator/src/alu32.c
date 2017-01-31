#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void alu32_init(ALU32 *alu32, Path *ops, Bus *a, Bus *b, Bus *s, Path *zero, Path *overflow){
	int i;

	Path *c;
	c = (Path *)malloc(sizeof(Path) * 32);

	for(i = 0; i < 32; i++){
		path_init(c+i);
	}

	Bus *less;
	less = (Bus *)malloc(sizeof(Bus));
	bus_init(less, 31);

	Path *set;
	set = (Path *)malloc(sizeof(Path));	
	path_init(set);

	alu32 -> alus = (ALU *)malloc(sizeof(ALU) * 31);
	alu32 -> alumsb = (ALU_MSB *)malloc(sizeof(ALU_MSB));

	alu_init(alu32 -> alus, ops, ops+1, ops+2, a -> p, b -> p, set, ops+2, s -> p, c);
	for(i = 1; i < 31; i++){
		alu_init(alu32 -> alus+i, ops, ops+1, ops+2, a -> p+i, b -> p+i, less -> p+i-1, c+i-1, s -> p+i, c+i);
	}
	alu_msb_init(alu32 -> alumsb, ops, ops+1, ops+2, a -> p+31, b -> p+31, less -> p+30, c+30, s -> p+31, c+31, set, overflow);

	// Zero判定
	alu32 -> ogaten = (ORGateN *)malloc(sizeof(ORGateN));
	alu32 -> ngate = (NOTGate *)malloc(sizeof(NOTGate));

	Path *inner;				//ORGateN -> NOTGate 
	inner = (Path *)malloc(sizeof(Path));
	path_init(inner);

	Path **po = (Path **)malloc(sizeof(Path*) * 32);
	for(i = 0; i < 32; i++){
		po[i] = s -> p+i;
	}

	orgaten_init(alu32 -> ogaten, po, inner, 32);
	notgate_init(alu32 -> ngate, inner, zero);

}

void alu32_run(ALU32 *alu32){
	int i, j;
	for(j = 0; j < 2; j++){	
		for(i = 0; i < 31; i++){
			alu_run(alu32 -> alus+i);
		}
		alu_msb_run(alu32 -> alumsb);
	}
	orgaten_run(alu32 -> ogaten);
	notgate_run(alu32 -> ngate);
}

void alu32_release(ALU32 *alu32){
	free(alu32 -> alus -> f1 -> ogate1 -> out1);
	free(alu32 -> alumsb -> f1 -> xorc2 -> ogate1 -> out1);

	int i;
	for(i = 0; i < 31; i++){
		alu_release(alu32 -> alus+i);
	}
	alu_msb_release(alu32 -> alumsb);

	free(alu32 -> alus);
	free(alu32 -> alumsb);

	free(alu32 -> ogaten -> ins);
	free(alu32 -> ogaten);
}

void alu32_driver(int na, int nb, Signal o1, Signal o2, Signal bin){
	ALU32 alu32;
	Bus a, b, s;
	Path *ops;
	Path zero, overflow;
	
	bus_init(&a, 32); bus_init(&b, 32); bus_init(&s, 32);
	bus_set_value(&a, na); bus_set_value(&b, nb);

	ops = (Path *)malloc(sizeof(Path) * 3);
	path_init(ops); path_init(ops+1); path_init(ops+2);
	path_set_signal(ops, o1); path_set_signal(ops+1, o2); path_set_signal(ops+2, bin);
	path_init(&zero); path_init(&overflow);
	
	alu32_init(&alu32, ops, &a, &b, &s, &zero, &overflow);

	alu32_run(&alu32);
	printf("ALU32(op: %d, %d)(bin: %d)(a: %d, b: %d) => result: %d, zero: %d, overflow: %d\n", 
			o2, o1, bin, na, nb, bus_get_value(&s), path_get_signal(&zero), path_get_signal(&overflow));
	alu32_release(&alu32);
	free(ops);
}