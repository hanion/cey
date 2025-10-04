#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int* 𐰓𐰃𐰔𐰃 = (int*)malloc(5 * sizeof(int));
	for (int 𐰃 = 0; 𐰃 < 5; ++𐰃) {
		𐰓𐰃𐰔𐰃[𐰃] = 𐰃 * 2;
	}
	printf("𐰢𐰀𐰼𐰴𐰀𐰉𐰀 𐰲*\n");
	free(𐰓𐰃𐰔𐰃);
	return 0;
}


