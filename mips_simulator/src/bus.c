#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

// Create an n-bit bus
void bus_init(Bus *b, int n){
	int i;
	b -> num = n;
	b -> p = (Path *)malloc(sizeof(Path) * n);

	for(i = 0; i < n; i++){
		path_init(b -> p+i);
	}
}

// Insert value into bus = turn into binary
void bus_set_value(Bus *b, int val){
	int i;
	for (i = 0; i < b->num; i++){
		if ((val & 0x1) == 1)
			path_set_signal(b -> p + i, true);
		else
			path_set_signal(b -> p + i, false);
		val >>= 1;
	}
	
}

// Return the value back
int bus_get_value(Bus *b){
	int i; int val = 0;
	for (i = b-> num-1; i>=0; i--){
		val <<= 1;
		if (path_get_signal(b -> p + i)){
			val += 1;
		}
	}
	return val;
}

// Take the i-th path
Path* bus_get_path(Bus *b, int i){
	return b -> p+i;
}

void bus_release(Bus *b){
	free(b -> p);
}

