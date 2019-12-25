#include <stdio.h>
#include <stdlib.h>

int issixdigits(int n);
int hasmatchingadjecentdigits(int n);
int digitsneverdecrease(int n);

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "filename argument missing\n");
		exit(1);
	}

	FILE *fp;
	if ((fp = fopen(*++argv, "r")) == NULL) {
		fprintf(stderr, "error opening file: %s\n", *argv);
		exit(1);
	}

	int start, end;
	if (fscanf(fp, "%d-%d", &start, &end) != 2) {
		fprintf(stderr, "invalid data in file: %s\n", *argv);
	}

	int count = 0;
	for (int i = start + 1; i < end; i++) {
		if (issixdigits(i) && hasmatchingadjecentdigits(i) && digitsneverdecrease(i)) {
			count++;
		}
	}

	printf("%d\n", count);
}

int issixdigits(int n) {
	return (n > 99999 && n < 1000000) ? 1 : 0;
}

int hasmatchingadjecentdigits(int n) {
	while (n) {
		if (n % 10 == (n / 10) % 10) {
			return 1;
		}
		n /= 10;
	}
	return 0;
}

int digitsneverdecrease(int n) {
	while (n) {
		if (n % 10 < (n / 10) % 10) {
			return 0;
		}
		n /= 10;
	}
	return 1;
}
