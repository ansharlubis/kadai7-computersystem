#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void alu_init(ALU *alu, Path *op2, Path *op1, Path *a, Path *b, Path *bin, Path *carryin, Path *s, Path *carryout){
	// operator: 01 (op2 = 0, op1 = 1)

	Path *inner1; 
	Path *inner2;
	Path *inner3;

	// addition for version 2
	Path *inner4;
	Path *inner5;

	Path *null;			// fourth input for MUX4
	inner1 = (Path *)malloc(sizeof(Path));
	inner2 = (Path *)malloc(sizeof(Path));
	inner3 = (Path *)malloc(sizeof(Path));
	inner4 = (Path *)malloc(sizeof(Path));
	inner5 = (Path *)malloc(sizeof(Path));
	null = (Path *)malloc(sizeof(Path));

	path_init(inner1); path_init(inner2); path_init(inner3); 
	path_init(inner4); path_init(inner5); path_init(null);

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
	mux4_init(alu -> mux4, op2, op1, inner1, inner2, inner3, null, s);	
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
	free(alu -> agate1 -> out1);
	free(alu -> ogate1 -> out1);
	free(alu -> f1 -> ogate1 -> out1);
	free(alu -> ngate1 -> out1);
	free(alu -> binvert -> ogate1 -> out1);
	free(((alu -> mux4) -> agatens+3) -> ins[2]);

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