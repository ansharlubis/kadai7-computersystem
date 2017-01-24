#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void cla4_init(CLA4 *cla4, Bus *a, Bus *b, Path *carryin, Bus *s, Path *carryout, Path *G, Path *P){
	int i;

	// Generator, propagator, and carryin buses' initialization
	Bus *g, *p, *c;
	g = (Bus *)malloc(sizeof(Bus));
	p = (Bus *)malloc(sizeof(Bus));
	c = (Bus *)malloc(sizeof(Bus));

	bus_init(g, 4);
	bus_init(p, 4);
	
		// initialization for carry in bus
		c -> num = 4;
		c -> p = (Path *)malloc(sizeof(Path) * 4);
		path_set_signal(c -> p, path_get_signal(carryin));

		for(i = 1; i < 4; i++){
			path_init(c -> p+i);
		}

	// PFA initialization
	cla4 -> pfa = (PFA *)malloc(sizeof(PFA)*4);

	for(i = 0; i < 4; i++){
		pfa_init(cla4 -> pfa+i, a -> p+i, b -> p+i, c -> p+i, g -> p+i, p -> p+i, s -> p+i);
	}

	// CLU initialization
	cla4 -> clu = (CLU *)malloc(sizeof(CLU));

	clu_init(cla4 -> clu, g, p, c, carryout);

	// ANDGateN and ORGateN initialization for P and G
	cla4 -> agaten = (ANDGateN *)malloc(sizeof(ANDGateN)*4);
	cla4 -> ogaten = (ORGateN *)malloc(sizeof(ORGateN));

	Path **pg1, **pg2, **pg3, **pg4, **pp1;
	pg1 = (Path **)malloc(sizeof(Path*) * 2);
	pg2 = (Path **)malloc(sizeof(Path*) * 3);
	pg3 = (Path **)malloc(sizeof(Path*) * 4);
	pg4 = (Path **)malloc(sizeof(Path*) * 4);
	pp1 = (Path **)malloc(sizeof(Path*) * 4);

	Path *inner1, *inner2, *inner3;
	inner1 = (Path *)malloc(sizeof(Path));
	inner2 = (Path *)malloc(sizeof(Path));
	inner3 = (Path *)malloc(sizeof(Path));
	path_init(inner1); path_init(inner2); path_init(inner3);

	pg1[0] = p -> p+3; pg1[1] = g -> p+2;
	pg2[0] = p -> p+3; pg2[1] = p -> p+2; pg2[2] = g -> p+1;
	pg3[0] = p -> p+3; pg3[1] = p -> p+2; pg3[2] = p -> p+1; pg3[3] = g -> p;
	pg4[0] = g -> p+3; pg4[1] = inner1	; pg4[2] = inner2  ; pg4[3] = inner3;
	pp1[0] = p -> p+3; pp1[1] = p -> p+2; pp1[2] = p -> p+1; pp1[3] = p -> p;

	andgaten_init(cla4 -> agaten, pg1, inner1, 2);
	andgaten_init(cla4 -> agaten+1, pg2, inner2, 3);
	andgaten_init(cla4 -> agaten+2, pg3, inner3, 4);
	orgaten_init(cla4 -> ogaten, pg4, G, 4);
	andgaten_init(cla4 -> agaten+3, pp1, P, 4);
}

void cla4_run(CLA4 *cla4){
	int i;

	for(i = 0; i < 4; i++){
		pfa_run(cla4 -> pfa+i);
	}
	clu_run(cla4 -> clu);
	andgaten_run(cla4 -> agaten);
	andgaten_run(cla4 -> agaten+1);
	andgaten_run(cla4 -> agaten+2);
	orgaten_run(cla4 -> ogaten);
	andgaten_run(cla4 -> agaten+3);

	for(i = 0; i < 4; i++){
		pfa_sum(cla4 -> pfa+i);
	}
}

void cla4_release(CLA4 *cla4){
	free(cla4 -> pfa -> agate1 -> in1);
	free(cla4 -> pfa -> agate1 -> in2);
	free(cla4 -> clu -> agates -> in2);

	free(cla4 -> agaten -> out1);
	free(cla4 -> ogaten -> ins);
	free(cla4 -> agaten -> ins);
	
	free(cla4 -> agaten);
	free(cla4 -> ogaten);

	clu_release(cla4 -> clu);
	free(cla4 -> clu);

	pfa_release(cla4 -> pfa);
	free(cla4 -> pfa);
}

void cla4_driver(int na, int nb){
	CLA4 cla4;
	Bus a, b, s;
	Path carry_in, carry_out, G, P;

	path_init(&carry_in); path_init(&carry_out);
	path_init(&G); path_init(&P);

	bus_init(&a, 4); bus_init(&b, 4); bus_init(&s, 4);
	bus_set_value(&a, na); bus_set_value(&b, nb);

	cla4_init(&cla4, &a, &b, &carry_in, &s, &carry_out, &G, &P);

	cla4_run(&cla4);
	printf("4 bits Carry Lookahead(%d, %d) => (Result: %d, carry out: %d)\n",
		na, nb, bus_get_value(&s), path_get_signal(&carry_out));
	
	cla4_release(&cla4);
}