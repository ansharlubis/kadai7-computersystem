#include <stdio.h>
#include <mips.h>

void orgate_init(ORGate *ogate, Path *in1, Path *in2, Path *out1){
	ogate -> in1 = in1;
	ogate -> in2 = in2;
	ogate -> out1 = out1;
}

void orgate_run(ORGate *ogate){
	Signal s1, s2, s3;
	s1 = path_get_signal(ogate -> in1);
	s2 = path_get_signal(ogate -> in2);
	s3 = s1 || s2;
	path_set_signal(ogate -> out1, s3);
}


void orgate_driver(Signal a, Signal b){
	Path in1, in2, out1;
	ORGate ogate;

	path_init(&in1);
	path_init(&in2);
	path_init(&out1);

	path_set_signal(&in1, a);
	path_set_signal(&in2, b);

	orgate_init(&ogate, &in1, &in2, &out1);

	orgate_run(&ogate);

	printf("AND(%d, %d) => %d\n", a, b, path_get_signal(&out1));
}