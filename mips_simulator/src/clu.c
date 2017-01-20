#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void clu_init(CLU *clu, Bus *g, Bus *p, Bus *c, Path *carryout){
	int i;
	Path *inner;
	inner = (Path *)malloc(sizeof(Path) * 26);
	clu -> agates = (ANDGate *)malloc(sizeof(ANDGate) * 20);
	clu -> ogates = (ORGate *)malloc(sizeof(ORGate) * 10);

	for (i = 0; i < 26; i++){
		path_init(inner+i);
	}

	//circuit for c1
	andgate_init(clu -> agates, p -> p, c -> p, inner);
	orgate_init(clu -> ogates, g -> p, inner, c -> p+1);

	//circuit for c2
	andgate_init(clu -> agates+1, g -> p, p -> p+1, inner+1);
	orgate_init(clu -> ogates+1, g -> p+1, inner+1, inner+2);

	andgate_init(clu -> agates+2, p -> p, p -> p+1, inner+3);
	andgate_init(clu -> agates+3, inner+3, c -> p, inner+4);
	orgate_init(clu -> ogates+2, inner+2, inner+4, c -> p+2);

	//circuit for c3
	andgate_init(clu -> agates+4, g -> p+1, p -> p+2, inner+5);
	orgate_init(clu -> ogates+3, g -> p+2, inner+5, inner+6);

	andgate_init(clu -> agates+5, g -> p, p -> p+1, inner+7);
	andgate_init(clu -> agates+6, inner+7, p -> p+2, inner+8);
	orgate_init(clu -> ogates+4, inner+6, inner+8, inner+9);
	
	andgate_init(clu -> agates+7, p -> p, p -> p+1, inner+10);
	andgate_init(clu -> agates+8, inner+10, p -> p+2, inner+11);
	andgate_init(clu -> agates+9, inner+11, c -> p, inner+12);
	orgate_init(clu -> ogates+5, inner+9, inner+12, c -> p+3);

	//circuit for carryout
	andgate_init(clu -> agates+10, g -> p+2, p -> p+3, inner+13);
	orgate_init(clu -> ogates+6, g -> p+3, inner+13, inner+14);

	andgate_init(clu -> agates+11, g -> p+1, p -> p+2, inner+15);
	andgate_init(clu -> agates+12, inner+15, p -> p+3, inner+16);
	orgate_init(clu -> ogates+7, inner+14, inner+16, inner+17);
	
	andgate_init(clu -> agates+13, g -> p, p -> p+1, inner+18);
	andgate_init(clu -> agates+14, inner+18, p -> p+2, inner+19);
	andgate_init(clu -> agates+15, inner+19, p -> p+3, inner+20);
	orgate_init(clu -> ogates+8, inner+17, inner+20, inner+21);

	andgate_init(clu -> agates+16, p -> p, p -> p+1, inner+22);
	andgate_init(clu -> agates+17, inner+22, p -> p+2, inner+23);
	andgate_init(clu -> agates+18, inner+23, p -> p+3, inner+24);
	andgate_init(clu -> agates+19, inner+24, c -> p, inner+25);
	orgate_init(clu -> ogates+9, inner+21, inner+25, carryout);
}

void clu_run(CLU *clu){
	int i;

	for(i = 0; i < 20; i++){
		andgate_run(clu -> agates+i);
	}

	for(i = 0; i < 10; i++){
		orgate_run(clu -> ogates+i);
	}
}

void clu_release(CLU *clu){
	free(clu -> agates -> out1);
	free(clu -> agates);
	free(clu -> ogates);
}

void clu_driver(int ng, int np){
	CLU clu;
	Bus g, p, c;
	Path carry_out;

	path_init(&carry_out);

	bus_init(&g, 4); bus_init(&p, 4); bus_init(&c, 4);
	bus_set_value(&g, ng); bus_set_value(&p, np);

	Bus *t = &c;
	path_init(t -> p);

	clu_init(&clu, &g, &p, &c, &carry_out);

	clu_run(&clu);

	printf("CLU(%d, %d) => (Carry in: %d, Carry out: %d)\n", ng, np, bus_get_value(&c),	path_get_signal(&carry_out));

	clu_release(&clu);
}