#include <stdio.h>
#include <stdlib.h>

#define ADD_CODE 1
#define MULT_CODE 2
#define INPUT_CODE 3
#define OUTPUT_CODE 4
#define END_CODE 99

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "filename argument missing\n");
		exit(1);
	}

	FILE *fp;
	if ((fp = fopen(*++argv, "r")) == NULL) {
		fprintf(stderr, "error opening file: %s", *argv);
		exit(1);
	} 

	int code, numinst = 0;
	while (fscanf(fp, "%d,", &code) == 1) {
		numinst++;
	}
	rewind(fp);

	int *codes = (int *)malloc(numinst * sizeof(int));
	for (int i = 0; i < numinst && fscanf(fp, "%d,", codes + i) == 1; i++) {}

	int ip = 0;
	while (ip < numinst) {
		int inst = codes[ip];
		int opcode = inst % 100;

		if (opcode == END_CODE) {
			break;
		} else if (opcode == ADD_CODE) {
			int firstparam = (inst / 100) % 10 ? codes[ip+1] : codes[codes[ip+1]];
			int secondparam = (inst / 1000) % 10 ? codes[ip+2] : codes[codes[ip+2]];
			codes[codes[ip+3]] = firstparam + secondparam;
			ip += 4;
		} else if (opcode == MULT_CODE) {
			int firstparam = (inst / 100) % 10 ? codes[ip+1] : codes[codes[ip+1]];
			int secondparam = (inst / 1000) % 10 ? codes[ip+2] : codes[codes[ip+2]];
			codes[codes[ip+3]] = firstparam * secondparam;
			ip += 4;
		} else if (opcode == INPUT_CODE) {
			int input;
			scanf("%d", &input);
			codes[codes[ip+1]] = input;
			ip += 2;
		} else if (opcode == OUTPUT_CODE) {
			printf("%d\n", codes[codes[ip+1]]);
			ip += 2;
		} else {
			fprintf(stderr, "unknown int code: %d\n", codes[ip]);
			exit(1);
		}
	}

	exit(0);
}
