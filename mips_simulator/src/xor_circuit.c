#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void xorcircuit_init(XORCircuit *xorc, Path *in1, Path *in2, Path *out1){
	Path *inner1;
	Path *inner2;
	Path *inner3;
	Path *inner4;

	inner1 = (Path *)malloc(sizeof(Path));
	inner2 = (Path *)malloc(sizeof(Path));
	inner3 = (Path *)malloc(sizeof(Path));
	inner4 = (Path *)malloc(sizeof(Path));
	path_init(inner1);
	path_init(inner2);
	path_init(inner3);
	path_init(inner4);

	xorc -> ngate1 = (NOTGate *)malloc(sizeof(NOTGate));
	xorc -> ngate2 = (NOTGate *)malloc(sizeof(NOTGate));
	xorc -> agate1 = (ANDGate *)malloc(sizeof(ANDGate));
	xorc -> agate2 = (ANDGate *)malloc(sizeof(ANDGate));
	xorc -> ogate1 = (ORGate *)malloc(sizeof(ORGate));

	notgate_init(xorc -> ngate1, in1, inner1);
	notgate_init(xorc -> ngate2, in2, inner2);
	andgate_init(xorc -> agate1, inner1, in2, inner3);
	andgate_init(xorc -> agate2, in1, inner2, inner4);
	orgate_init(xorc -> ogate1, inner3, inner4, out1);
}	

void xorcircuit_run(XORCircuit *xorc){
	notgate_run(xorc -> ngate1);
	notgate_run(xorc -> ngate2);
	andgate_run(xorc -> agate1);
	andgate_run(xorc -> agate2);
	orgate_run(xorc -> ogate1);
}

void xorcircuit_release(XORCircuit *xorc){
	free(xorc -> ngate1 -> out1);
	free(xorc -> ngate2 -> out1);
	free(xorc -> agate1 -> out1);
	free(xorc -> agate2 -> out1);
	free(xorc -> ngate1);
	free(xorc -> ngate2);
	free(xorc -> agate1);
	free(xorc -> agate2);
	free(xorc -> ogate1);
}

Path* xorcircuit_get_out_path(XORCircuit *xorc){
	return xorc -> ogate1 -> out1;
}

void xorcircuit_driver(Signal a, Signal b){
	Path in1, in2, out1;
	XORCircuit xorc;

	path_init(&in1);
	path_init(&in2);
	path_init(&out1);

	path_set_signal(&in1, a);
	path_set_signal(&in2, b);

	xorcircuit_init(&xorc, &in1, &in2, &out1);

	xorcircuit_run(&xorc);

	printf("XORC(%d, %d) => %d\n", a, b, path_get_signal(xorcircuit_get_out_path(&xorc)));

	xorcircuit_release(&xorc);
}