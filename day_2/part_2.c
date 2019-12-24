#include <stdio.h>
#include <stdlib.h>

#define ADD_CODE 1
#define MULT_CODE 2
#define END_CODE 99
#define RESULT 19690720
#define MAX_CODES 1024

void copycodes(int from[], int to[], int len);

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "filename argument missing\n");
		exit(1);
	}

	FILE *fp;
	if ((fp = fopen(*++argv, "r")) == NULL) {
		fprintf(stderr, "error opening file: %s", *argv);
		exit(1);
	} else {
		int initcodes[MAX_CODES];
		int len;
		for (len = 0; len < MAX_CODES; len++) {
			if (fscanf(fp, "%d,", &initcodes[len]) != 1) {
				break;
			}
		}

		int codes[MAX_CODES];
		for (int noun = 0; noun < 100; noun++) {
			for (int verb = 0; verb < 100; verb++) {
				copycodes(initcodes, codes, len);
				codes[1] = noun;
				codes[2] = verb;

				for (int step = 0; step < len; step += 4) {
					if (codes[step] == END_CODE) {
						if (codes[0] == RESULT) {
							printf("noun: %d, verb: %d, 100 * noun + verb: %d\n", noun, verb, 100 * noun + verb);
							exit(0);
						}
						break;
					} else if (codes[step] == ADD_CODE) {
						codes[codes[step+3]] = codes[codes[step+1]] + codes[codes[step+2]];
					} else if (codes[step] == MULT_CODE) {
						codes[codes[step+3]] = codes[codes[step+1]] * codes[codes[step+2]];
					} else {
						fprintf(stderr, "unknown int code: %d\n", codes[step]);
						exit(1);
					}
				}
			}
		}
	}

	fprintf(stderr, "unable to find noun and verb that results in %d\n", RESULT);
	exit(1);
}

void copycodes(int from[], int to[], int len) {
	for (int i = 0; i < len; i++) {
		to[i] = from[i];
	}
}
