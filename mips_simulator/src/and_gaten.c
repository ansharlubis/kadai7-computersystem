#include <stdio.h>
#include <mips.h>

void andgaten_init(ANDGateN *agaten, Path **ins, Path *out1, int num){
	agaten -> num = num;
	agaten -> ins = ins;
	agaten -> out1 = out1;
}

void andgaten_run(ANDGateN *agaten){
	int i;
	Signal val = true;

	for(i = 0; i < agaten -> num; i++){
		if(path_get_signal(agaten -> ins[i]) == false){
			val = false;
			break;
		}
	}

	path_set_signal(agaten -> out1, val);	
}

