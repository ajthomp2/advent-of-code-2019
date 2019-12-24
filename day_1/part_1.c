#include <stdio.h>
#include <stdlib.h>

int calcfuel(int mass);

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "missing filename argument\n");
		exit(1);
	}

	FILE *fp;
	int modsize, total = 0;
	if ((fp = fopen(*++argv, "r")) == NULL) {
		fprintf(stderr, "error opening file: %s\n", *argv);
		exit(1);
	} else {
		while (fscanf(fp, "%d", &modsize) != EOF) {
			total += modsize / 3 - 2;
		}

		printf("total: %d\n", total);

		fclose(fp);
	}
}
