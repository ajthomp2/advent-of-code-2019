#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum opcodes {
	add = 1,
	mult = 2,
	input = 3,
	output = 4,
	jumpiftrue = 5,
	jumpiffalse = 6,
	lessthan = 7,
	equals = 8,
	end = 99
};

int calcparam(int ip, int *codes, int offset);

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
	int done = 0;
	while (ip < numinst && !done) {
		int inst = codes[ip];
		int opcode = inst % 100;

		switch (opcode) {
			case end:
				done = 1;
				break;
			case add: {
				codes[codes[ip+3]] = calcparam(ip, codes, 1) + calcparam(ip, codes, 2);
				ip += 4;
				break;
			}
			case mult: {
				codes[codes[ip+3]] = calcparam(ip, codes, 1) * calcparam(ip, codes, 2);
				ip += 4;
				break;
			}
			case input: {
				int input;
				scanf("%d", &input);
				codes[codes[ip+1]] = input;
				ip += 2;
				break;
			}
			case output: {
				printf("%d\n", codes[codes[ip+1]]);
				ip += 2;
				break;
		    }
			case jumpiftrue: {
				ip = calcparam(ip, codes, 1) ? calcparam(ip, codes, 2) : ip + 3;
				break;
			}
			case jumpiffalse: {
				ip = !calcparam(ip, codes, 1) ? calcparam(ip, codes, 2) : ip + 3;
				break;
			}
			case lessthan: {
				codes[codes[ip+3]] = calcparam(ip, codes, 1) < calcparam(ip, codes, 2) ? 1 : 0;
				ip += 4;
				break;
			}
			case equals: {
				codes[codes[ip+3]] = calcparam(ip, codes, 1) == calcparam(ip, codes, 2) ? 1 : 0;
				ip += 4;
				break;
		    }
			default:
				fprintf(stderr, "unknown int code: %d\n", codes[ip]);
				exit(1);
				break;
		}
	}

	exit(0);
}

int calcparam(int ip, int *codes, int offset) {
	int immediate = codes[ip] / (10 * (int)pow(10, offset)) % 10;
	return immediate ? codes[ip + offset] : codes[codes[ip + offset]];
}
