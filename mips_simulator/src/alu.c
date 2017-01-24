#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void alu_init(ALU *alu, Path *op1, Path *op2, Path *bin, Path *a, Path *b, Path *less, Path *carryin, Path *s, Path *carryout){
	// operator: 01 (op1 = 1, op2 = 0)

	Path *inner1;	// agate1 -> mux4
	Path *inner2; 	// ogate1 -> mux4
	Path *inner3; 	// f1 -> mux4
	Path *inner4;	// ngate1 -> binvert
	Path *inner5;	// binvert -> f1
	
	inner1 = (Path *)malloc(sizeof(Path));
	inner2 = (Path *)malloc(sizeof(Path));
	inner3 = (Path *)malloc(sizeof(Path));
	inner4 = (Path *)malloc(sizeof(Path));
	inner5 = (Path *)malloc(sizeof(Path));
	
	path_init(inner1); path_init(inner2); path_init(inner3); 
	path_init(inner4); path_init(inner5); 

	alu -> agate1 = (ANDGate *)malloc(sizeof(ANDGate));
	alu -> ogate1 = (ORGate *)malloc(sizeof(ORGate));
	alu -> ngate1 = (NOTGate *)malloc(sizeof(NOTGate));
	alu -> f1 = (FA *)malloc(sizeof(FA));
	alu -> mux4 = (MUX4 *)malloc(sizeof(MUX4));
	alu -> binvert = (MUX *)malloc(sizeof(MUX));

	notgate_init(alu -> ngate1, b, inner4);
	mux_init(alu -> binvert, bin, b, inner4, inner5);
	andgate_init(alu -> agate1, a, inner5, inner1);
	orgate_init(alu -> ogate1, a, inner5, inner2);
	fa_init(alu -> f1, a, inner5, carryin, inner3, carryout);
	mux4_init(alu -> mux4, op1, op2, inner1, inner2, inner3, less, s);	
}

void alu_run(ALU *alu){
	notgate_run(alu -> ngate1);
	mux_run(alu -> binvert);
	andgate_run(alu -> agate1);
	orgate_run(alu -> ogate1);
	fa_run(alu -> f1);
	mux4_run(alu -> mux4);
}

void alu_release(ALU *alu){
	free(alu -> agate1 -> out1);					// inner1
	free(alu -> ogate1 -> out1);					// inner2
	free(alu -> f1 -> xorc2 -> ogate1 -> out1);		// inner3
	free(alu -> ngate1 -> out1);					// inner4
	free(alu -> binvert -> ogate1 -> out1);			// inner5
	
	fa_release(alu -> f1);
	mux4_release(alu -> mux4);
	mux_release(alu -> binvert);

	free(alu -> agate1);
	free(alu -> ogate1);
	free(alu -> ngate1);
	free(alu -> f1);
	free(alu -> mux4);
	free(alu -> binvert);
}

void alu_driver(Signal in1, Signal in2, Signal o1, Signal o2, Signal bin, Signal l){
	ALU alu;
	Path a, b, op1, op2, binvert, less, carryout, s;
	path_init(&a); 
	path_init(&b); 
	path_init(&op1); 
	path_init(&op2);
	path_init(&binvert); 
	path_init(&less); 
	path_init(&carryout); 
	path_init(&s);

	path_set_signal(&a, in1); 
	path_set_signal(&b, in2); 
	path_set_signal(&op1, o1); 
	path_set_signal(&op2, o2); 
	path_set_signal(&binvert, bin);
	path_set_signal(&less, l); 

	alu_init(&alu, &op1, &op2, &binvert, &a, &b, &less, &binvert, &s, &carryout);

	alu_run(&alu);
	printf("ALU(op: %d, %d)(bin: %d)(less: %d)(%d, %d) => %d, %d\n", o2, o1, bin, l, in1, in2, path_get_signal(&s), path_get_signal(&carryout));
	alu_release(&alu);
}

