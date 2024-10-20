// # Copyright Stamatin Teodor 315CA 2022 - 2023
#include "function.h"
#include <stdio.h>
#include <stdlib.h>

int **calloc_matrix(int n, int m)
{
	int **a = (int **)calloc(n, sizeof(int *));
	if (!a) {
		fprintf(stderr, "calloc() failed\n");
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		a[i] = (int *)calloc(m, sizeof(int));
		if (!a) {
			fprintf(stderr, "calloc() failed\n");
			while (--i >= 0)
				free(a[i]);
			free(a);
			return NULL;
		}
	}
	return a;
}

void free_matrix(int n, int **a)
{
	for (int i = 0; i < n; i++)
		free(a[i]);
	free(a);
}

int **alloc_matrix(int n, int m)
{
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		a[i] = (int *)malloc(m * sizeof(int));
		if (!a) {
			fprintf(stderr, "malloc() failed\n");
			while (--i >= 0)
				free(a[i]);
			free(a);
			return NULL;
		}
	}
	return a;
}