#include <stdio.h>
#include <stdlib.h>

#define ADD_CODE 1
#define MULT_CODE 2
#define END_CODE 99
#define MAX_CODES 1024

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "filename argument missing\n");
		exit(1);
	}

	FILE *fp;
	if ((fp = fopen(*++argv, "r")) == NULL) {
		fprintf(stderr, "error opening file: %s", *argv);
		exit(2);
	} else {
		int codes[MAX_CODES];
		int i;
		for (i = 0; i < MAX_CODES; i++) {
			if (fscanf(fp, "%d,", &codes[i]) != 1) {
				break;
			}
		}

		for (int j = 0; j < i; j += 4) {
			if (codes[j] == END_CODE) {
				printf("result: %d\n", codes[0]);
				break;
			} else if (codes[j] == ADD_CODE) {
				codes[codes[j+3]] = codes[codes[j+1]] + codes[codes[j+2]];
			} else if (codes[j] == MULT_CODE) {
				codes[codes[j+3]] = codes[codes[j+1]] * codes[codes[j+2]];
			} else {
				fprintf(stderr, "unknown int code: %d\n", codes[j]);
				exit(3);
			}
		}
	}

	exit(0);
}
