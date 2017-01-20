#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void rca_init(RCA *r, Bus *a, Bus *b, Path *carryin, Bus *s, Path *carryout){
	int i;
	Path *p;
	p = (Path *)malloc(sizeof(Path) * 31);
	r -> f = (FA *)malloc(sizeof(FA) * 32);

	for (i = 0; i < 31; i++){
		path_init(p+i);
	}

	fa_init(r -> f, a -> p, b -> p, carryin, s -> p, p);
	for (i = 1; i < 31; i++){
		fa_init(r -> f+i, a -> p+i, b -> p+i, p+i-1, s -> p+i, p+i);
	}
	fa_init(r -> f+31, a -> p+31, b -> p+31, p+30, s -> p+31, carryout);
}

void rca_run(RCA *r){
	int i;
	for (i = 0; i < 32; i++){
		fa_run(r -> f+i);
	}
}

void rca_release(RCA *r){
	int i;
	for(i = 0; i < 32; i++){
		fa_release(r -> f+i);
	}
	
	free(r -> f -> ogate1 -> out1);
	free(r -> f);
}

void rca_driver(int na, int nb){
	RCA r;
	Bus a, b, s;
	Path carry_in, carry_out;

	// Path initialization
	path_init(&carry_in); path_init(&carry_out);

	// Bus initialization and setting value
	bus_init(&a, 32); bus_init(&b, 32); bus_init(&s, 32);
	bus_set_value(&a, na); bus_set_value(&b, nb);

	// RCA initialization
	rca_init(&r, &a, &b, &carry_in, &s, &carry_out);

	rca_run(&r);
	printf("ADDER(%d, %d) => %d\n", na, nb, bus_get_value(&s));
	rca_release(&r);
}