#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void pfa_init(PFA *pfa, Path *a, Path *b, Path *carryin, Path *g, Path *p, Path *s){
	// not used
	Path *null;
	null = (Path *)malloc(sizeof(Path));
	path_init(null);

	pfa -> agate1 = (ANDGate *)malloc(sizeof(ANDGate));
	pfa -> ogate1 = (ORGate *)malloc(sizeof(ORGate));
	pfa -> f1 = (FA *)malloc(sizeof(FA));

	andgate_init(pfa -> agate1, a, b, g);
	orgate_init(pfa -> ogate1, a, b, p);
	fa_init(pfa -> f1, a, b, carryin, s, null);				
}

void pfa_run(PFA *pfa){
	andgate_run(pfa -> agate1);
	orgate_run(pfa -> ogate1);
}

void pfa_sum(PFA *pfa){
	fa_run(pfa -> f1);
}

void pfa_release(PFA *pfa){
	free(pfa -> agate1);
	free(pfa -> ogate1);
	free(pfa -> f1 -> ogate1 -> out1);
	fa_release(pfa -> f1);
	free(pfa -> f1);
}

void pfa_driver(Signal a, Signal b, Signal carry_in){
	PFA pfa;
	Path g, p, s;
	Path in1, in2, carryin;

	path_init(&g); path_init(&p); path_init(&s);
	path_init(&in1); path_init(&in2); path_init(&carryin);

	path_set_signal(&in1, a);
	path_set_signal(&in2, b);
	path_set_signal(&carryin, carry_in);

	pfa_init(&pfa, &in1, &in2, &carryin, &g, &p, &s);

	pfa_run(&pfa);

	// Carry out is not needed
	printf("PFA(a: %d, b: %d, Carry in: %d) => (G: %d, P: %d)\n", 
		a, b, carry_in, path_get_signal(&g), path_get_signal(&p));			

	pfa_release(&pfa);
}
