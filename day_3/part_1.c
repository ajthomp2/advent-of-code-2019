#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_PATH 1024

struct node {
	int x;
	int y;
};

int readpath(FILE *fp, struct node *path[]);
void getmapcoords(struct node *path1[], int path1len, struct node *path2[], int path2len, int *maxx, int *maxy, int *minx, int *miny);
void normalizepath(struct node *path[], int pathlen, int offsetx, int offsety);

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

	struct node* path1[MAX_PATH];
	int pathlen1 = readpath(fp, path1);
	struct node* path2[MAX_PATH];
	int pathlen2 = readpath(fp, path2);

	int maxx = 0, maxy = 0, minx = 0, miny = 0;
	getmapcoords(path1, pathlen1, path2, pathlen2, &maxx, &maxy, &minx, &miny);
	normalizepath(path1, pathlen1, abs(minx), abs(miny));
	normalizepath(path2, pathlen2, abs(minx), abs(miny));

	int rows = abs(maxy) + abs(miny) + 1;
	int cols = abs(maxx) + abs(minx) + 1;

	int *m = (int *)malloc(rows * cols * sizeof(int));
	for (int i = 1; i < pathlen1; i++) {
		struct node *pn = path1[i-1];
		struct node *n = path1[i];

		if (pn->x != n->x) {
			if (pn->x < n->x) {
				for (int j = pn->x + 1; j <= n->x; j++) {
					*(m + n->y * cols + j) = 1;
				}
			}
			if (pn->x > n->x) {
				for (int j = pn->x - 1; j >= n->x; j--) {
					*(m + n->y * cols + j) = 1;
				}
			}
		}
		if (pn->y != n->y) {
			if (pn->y < n->y) {
				for (int j = pn->y + 1; j <= n->y; j++) {
					*(m + j * cols + n->x) = 1;
				}
			}
			if (pn->y > n->y) {
				for (int j = pn->y - 1; j >= n->y; j--) {
					*(m + j * cols + n->x) = 1;
				}
			}
		}
	}
	for (int i = 1; i < pathlen2; i++) {
		struct node *pn = path2[i-1];
		struct node *n = path2[i];

		if (pn->x != n->x) {
			if (pn->x < n->x) {
				for (int j = pn->x + 1; j <= n->x; j++) {
					*(m + n->y * cols + j) += 1;
				}
			}
			if (pn->x > n->x) {
				for (int j = pn->x - 1; j >= n->x; j--) {
					*(m + n->y * cols + j) += 1;
				}
			}
		}
		if (pn->y != n->y) {
			if (pn->y < n->y) {
				for (int j = pn->y + 1; j <= n->y; j++) {
					*(m + j * cols + n->x) += 1;
				}
			}
			if (pn->y > n->y) {
				for (int j = pn->y - 1; j >= n->y; j--) {
					*(m + j * cols + n->x) += 1;
				}
			}
		}
	}

	int originx = path1[0]->x;
	int originy = path1[0]->y;
	printf("%d,%d\n", originx, originy);
	int mindist = INT_MAX;
	for (int i = rows - 1; i >= 0; i--) {
		for (int j = 0; j < cols; j++) {
			if (*(m + i * cols + j) > 1) {
				int dist = abs(originx - j) + abs(originy - i);
				if (dist < mindist) {
					mindist = dist;
				}
			}
		}
	}
	printf("%d\n", mindist);
}

int readpath(FILE *fp, struct node *path[]) {
	struct node *np = (struct node *) malloc(sizeof(struct node));
	np->x = 0;
	np->y = 0;
	path[0] = np;

	int len;
	char dir;
	int amount;
	for (len = 1; fscanf(fp, "%c%d,", &dir, &amount) == 2; len++) {
		np = (struct node *) malloc(sizeof(struct node));
		if (dir == 'U') {
			np->x = path[len-1]->x;
			np->y = path[len-1]->y + amount;
		} else if (dir == 'D') {
			np->x = path[len-1]->x;
			np->y = path[len-1]->y - amount;
		} else if (dir == 'R') {
			np->x = path[len-1]->x + amount;
			np->y = path[len-1]->y;
		} else if (dir == 'L') {
			np->x = path[len-1]->x - amount;
			np->y = path[len-1]->y;
		} else {
			fprintf(stderr, "unknown direction: %c", dir);
			exit(1);
		}
		path[len] = np;
	}

	return len;
}

void normalizepath(struct node *path[], int pathlen, int offsetx, int offsety) {
	for (int i = 0; i < pathlen; i++) {
		path[i]->x += offsetx;
		path[i]->y += offsety;
	}
}

void updateminandmax(struct node *n, int *maxx, int *maxy, int *minx, int *miny);

void getmapcoords(struct node *path1[], int path1len, struct node *path2[], int path2len, int *maxx, int *maxy, int *minx, int *miny) {
	for (int i = 0; i < path1len; i++) {
		updateminandmax(path1[i], maxx, maxy, minx, miny);
	}

	for (int i = 0; i < path2len; i++) {
		updateminandmax(path2[i], maxx, maxy, minx, miny);
	}
}

void updateminandmax(struct node *n, int *maxx, int *maxy, int *minx, int *miny) {
	if (n->x > *maxx) {
		*maxx = n->x;
	}
	if (n->x < *minx) {
		*minx = n->x;
	}
	if (n->y > *maxy) {
		*maxy = n->y;
	}
	if (n->y < *miny) {
		*miny = n->y;
	}
}
