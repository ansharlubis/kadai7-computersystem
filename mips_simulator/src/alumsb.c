#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void alu_msb_init(ALU_MSB *alu_msb, Path *op1, Path *op2, Path *bin, Path *a, Path *b, Path *less, Path *carryin, Path *s, Path *carryout, Path *set, Path *overflow){
	// operator: 01 (op1 = 1, op2 = 0)

	Path *inner1;	// agate1 -> mux4 
	Path *inner2;	// ogate1 -> mux4
	Path *inner4;	// ngate1 -> binvert
	Path *inner5;	// binvert -> f1

	inner1 = (Path *)malloc(sizeof(Path));
	inner2 = (Path *)malloc(sizeof(Path));
	inner4 = (Path *)malloc(sizeof(Path));
	inner5 = (Path *)malloc(sizeof(Path));
	
	path_init(inner1); path_init(inner2); 
	path_init(inner4); path_init(inner5); 

	alu_msb -> agate1 = (ANDGate *)malloc(sizeof(ANDGate));
	alu_msb -> ogate1 = (ORGate *)malloc(sizeof(ORGate));
	alu_msb -> ngate1 = (NOTGate *)malloc(sizeof(NOTGate));
	alu_msb -> f1 = (FA *)malloc(sizeof(FA));
	alu_msb -> mux4 = (MUX4 *)malloc(sizeof(MUX4));
	alu_msb -> binvert = (MUX *)malloc(sizeof(MUX));

	notgate_init(alu_msb -> ngate1, b, inner4);
	mux_init(alu_msb -> binvert, bin, b, inner4, inner5);
	andgate_init(alu_msb -> agate1, a, inner5, inner1);
	orgate_init(alu_msb -> ogate1, a, inner5, inner2);
	fa_init(alu_msb -> f1, a, inner5, carryin, set, carryout);
	mux4_init(alu_msb -> mux4, op1, op2, inner1, inner2, set, less, s);	

// Overflow判定
	alu_msb -> ngates = (NOTGate *)malloc(sizeof(NOTGate) * 4);
	alu_msb -> agatens = (ANDGateN *)malloc(sizeof(ANDGateN) * 4);
	alu_msb -> ogaten = (ORGateN *)malloc(sizeof(ORGateN));

	Path **ag1, **ag2, **ag3, **ag4;
	Path **og1;
	ag1 = (Path **)malloc(sizeof(Path*) * 4);		// ¬binvert∧¬A∧¬B∧ S
	ag2 = (Path **)malloc(sizeof(Path*) * 4);		// ¬binvert∧ A∧ B∧¬S
	ag3 = (Path **)malloc(sizeof(Path*) * 4);		// binvert ∧¬A∧ B∧ S
	ag4 = (Path **)malloc(sizeof(Path*) * 4);		// binvert ∧ A∧¬B∧¬S
	og1 = (Path **)malloc(sizeof(Path*) * 4);

	Path *inner;			
	// 0 ~ 3 for NOTGate 0 ~ 3, 4 ~ 7 for ORGateN 
	inner = (Path *)malloc(sizeof(Path) * 8);
	int i;
	for(i = 0; i < 8; i++){
		path_init(inner+i);
	}

	// NOTGate initialization
	notgate_init(alu_msb -> ngates, bin, inner);
	notgate_init(alu_msb -> ngates+1, a, inner+1);
	notgate_init(alu_msb -> ngates+2, b, inner+2);
	notgate_init(alu_msb -> ngates+3, s, inner+3);

	// ANDGateN initialization
	ag1[0] = inner; ag1[1] = inner+1; ag1[2] = inner+2; ag1[3] = s; 
	ag2[0] = inner; ag2[1] = a; 	  ag2[2] = b; 		ag2[3] = inner+3; 
	ag3[0] = bin; 	ag3[1] = inner+1; ag3[2] = b; 		ag3[3] = s;
	ag4[0] = bin; 	ag4[1] = a; 	  ag4[2] = inner+2; ag4[3] = inner+3;
	
	andgaten_init(alu_msb -> agatens, ag1, inner+4, 4);
	andgaten_init(alu_msb -> agatens+1, ag2, inner+5, 4);
	andgaten_init(alu_msb -> agatens+2, ag3, inner+6, 4);
	andgaten_init(alu_msb -> agatens+3, ag4, inner+7, 4);

	// ORGateN initialization
	og1[0] = inner+4; og1[1] = inner+5; og1[2] = inner+6; og1[3] = inner+7;

	orgaten_init(alu_msb -> ogaten, og1, overflow, 4);
}

void alu_msb_run(ALU_MSB *alu){
	int i;

	notgate_run(alu -> ngate1);
	mux_run(alu -> binvert);
	andgate_run(alu -> agate1);
	orgate_run(alu -> ogate1);
	fa_run(alu -> f1);
	mux4_run(alu -> mux4);

	for(i = 0; i < 4; i++){
		notgate_run(alu -> ngates+i);
	}
	for(i = 0; i < 4; i++){
		andgaten_run(alu -> agatens+i);
	}
	orgaten_run(alu -> ogaten);
}

void alu_msb_release(ALU_MSB *alu){
	free(alu -> agate1 -> out1);				// inner1
	free(alu -> ogate1 -> out1);				// inner2
	free(alu -> ngate1 -> out1);				// inner4
	free(alu -> binvert -> ogate1 -> out1);		// inner5
	
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

void alu_msb_driver(Signal in1, Signal in2, Signal o1, Signal o2, Signal bin, Signal carry_in){
	ALU_MSB alumsb;
	Path a, b, op1, op2, binvert, less, carryin;
	Path carryout, s, set, overflow;
	path_init(&a);
	path_init(&b);
	path_init(&op1);
	path_init(&op2);
	path_init(&binvert);
	path_init(&less);
	path_init(&carryin);
	path_init(&carryout);
	path_init(&s);
	path_init(&set);
	path_init(&overflow);

	path_set_signal(&a, in1); path_set_signal(&b, in2); 
	path_set_signal(&op1, o1); path_set_signal(&op2, o2); path_set_signal(&binvert, bin);
	path_set_signal(&carryin, carry_in);

	alu_msb_init(&alumsb, &op1, &op2, &binvert, &a, &b, &less, &carryin, &s, &carryout, &set, &overflow);

	alu_msb_run(&alumsb);
	printf("ALU_MSB(op: %d, %d)(bin: %d)(carryin: %d)(%d, %d) => rst: %d, set: %d\n", o2, o1, bin, carry_in, in1, in2, path_get_signal(&s), path_get_signal(&set));
	alu_msb_release(&alumsb);

}