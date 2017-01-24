#include <stdio.h>
#include <mips.h>
#include <stdlib.h>

void mux4_init(MUX4 *mux4, Path *ct1, Path *ct2, Path *in1, Path *in2, Path *in3, Path *in4, Path *out1){
	int i;
	Path *inner;
	inner = (Path *)malloc(sizeof(Path) * 8);
	for(i = 0; i < 8; i++){
		path_init(inner+i);
	}

	Path **pa1, **pa2, **pa3, **pa4, **po1;
	pa1 = (Path **)malloc(sizeof(Path*) * 3);
	pa2 = (Path **)malloc(sizeof(Path*) * 3);
	pa3 = (Path **)malloc(sizeof(Path*) * 3);
	pa4 = (Path **)malloc(sizeof(Path*) * 3);
	po1 = (Path **)malloc(sizeof(Path*) * 4);

	pa1[0] = inner	; pa1[1] = inner+1; pa1[2] = in1; 
	pa2[0] = ct1  	; pa2[1] = inner+2; pa2[2] = in2;
	pa3[0] = inner+3; pa3[1] = ct2	  ; pa3[2] = in3;
	pa4[0] = ct1	; pa4[1] = ct2	  ; pa4[2] = in4;
	po1[0] = inner+4; po1[1] = inner+5; po1[2] = inner+6; po1[3] = inner+7;

	mux4 -> agatens = (ANDGateN *)malloc(sizeof(ANDGateN) * 4);
	mux4 -> ogatens = (ORGateN *)malloc(sizeof(ORGateN));
	mux4 -> ngates = (NOTGate *)malloc(sizeof(NOTGate) * 4);

	notgate_init(mux4 -> ngates, ct1, inner);
	notgate_init(mux4 -> ngates+1, ct2, inner+1);
	notgate_init(mux4 -> ngates+2, ct2, inner+2);
	notgate_init(mux4 -> ngates+3, ct1, inner+3);
	andgaten_init(mux4 -> agatens, pa1, inner+4, 3);
	andgaten_init(mux4 -> agatens+1, pa2, inner+5, 3);
	andgaten_init(mux4 -> agatens+2, pa3, inner+6, 3);
	andgaten_init(mux4 -> agatens+3, pa4, inner+7, 3);
	orgaten_init(mux4 -> ogatens, po1, out1, 4);

}

void mux4_run(MUX4 *mux4){
	int i;

	for(i = 0; i < 4; i++){
		notgate_run(mux4 -> ngates+i);
	}

	for(i = 0; i < 4; i++){
		andgaten_run(mux4 -> agatens+i);
	}

	orgaten_run(mux4 -> ogatens);
}

void mux4_release(MUX4 *mux4){
	int i;

	free(mux4 -> ngates -> out1);  			// free inner	
	free(mux4 -> agatens -> ins); 			// free pa1
	free((mux4 -> agatens+1) -> ins);		// free pa2
	free((mux4 -> agatens+2) -> ins);		// free pa3
	free((mux4 -> agatens+3) -> ins);		// free pa4
	free(mux4 -> ogatens -> ins);			// free po1
	
	free(mux4 -> agatens);
	free(mux4 -> ogatens);
	free(mux4 -> ngates);
}

void mux4_driver(Signal c1, Signal c2, Signal i1, Signal i2, Signal i3, Signal i4){
	MUX4 mux;
	Path ct1, ct2, in1, in2, in3, in4, out1;
	path_init(&ct1); path_init(&ct2);
	path_init(&in1); path_init(&in2); path_init(&in3); path_init(&in4);

	path_set_signal(&ct1, c1); path_set_signal(&ct2, c2);
	path_set_signal(&in1, i1); path_set_signal(&in2, i2); path_set_signal(&in3, i3); path_set_signal(&in4, i4);

	mux4_init(&mux, &ct1, &ct2, &in1, &in2, &in3, &in4, &out1);

	mux4_run(&mux);
	printf("MUX4(%d, %d)(%d, %d, %d, %d) => %d\n", c2, c1, i1, i2, i3, i4, path_get_signal(&out1));
	mux4_release(&mux);

}