#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void mux_init(MUX *mux, Path *ct1, Path *in1, Path *in2, Path *out1){
	Path *inner1; 
	Path *inner2;
	Path *inner3;

	inner1 = (Path *)malloc(sizeof(Path));
	inner2 = (Path *)malloc(sizeof(Path));
	inner3 = (Path *)malloc(sizeof(Path));

	path_init(inner1);
	path_init(inner2);
	path_init(inner3);

	mux -> ngate1 = (NOTGate *)malloc(sizeof(NOTGate));
	mux -> agate1 = (ANDGate *)malloc(sizeof(ANDGate));
	mux -> agate2 = (ANDGate *)malloc(sizeof(ANDGate));
	mux -> ogate1 = (ORGate *)malloc(sizeof(ORGate));
	
	notgate_init(mux -> ngate1, ct1, inner1);
	andgate_init(mux -> agate1, inner1, in1, inner2);
	andgate_init(mux -> agate2, ct1, in2, inner3);
	orgate_init(mux -> ogate1, inner2, inner3, out1);

}

void mux_run(MUX *mux){
	notgate_run(mux -> ngate1);
	andgate_run(mux -> agate1);
	andgate_run(mux -> agate2);
	orgate_run(mux -> ogate1);
}

void mux_release(MUX *mux){
	free(mux -> ngate1 -> out1);
	free(mux -> agate1 -> out1);
	free(mux -> agate2 -> out1);
	free(mux -> ngate1);
	free(mux -> agate1);
	free(mux -> agate2);
	free(mux -> ogate1);
}

void mux_driver(Signal c1, Signal i1, Signal i2){
	MUX mux;
	Path ct1, in1, in2, out1;

	path_init(&ct1); path_init(&in1); path_init(&in2); path_init(&out1);
	path_set_signal(&ct1, c1); path_set_signal(&in1, i1); 
	path_set_signal(&in2, i2);

	mux_init(&mux, &ct1, &in1, &in2, &out1);
	
	mux_run(&mux);
	printf("MUX(%d)(%d, %d) => %d\n", c1, i1, i2, path_get_signal(&out1));
	mux_release(&mux);

}