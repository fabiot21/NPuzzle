#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "dict.h"
#include "bfs.h"

void displayMatrix(int matrix[dim][dim])
{
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}

	printf("\n");
}

void displayStringMatrix(char *state) {
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			printf("%c ", state[i*dim + j]);
		}
		printf("\n");
	}
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Modo de uso: ./solver test.txt\n");
		return 1;
	}


	///////////////////////////////////////////
	/////// INICIO LECTURA ARCHIVO ////////////
	///////////////////////////////////////////

	/* Abrimos el archivo en modo lectura */
	FILE* input_file = fopen(argv[1], "r");
	char help[100], *token;
	int i, j;
	/* Falló la apertura del archivo */
	if(!input_file)
	{
		printf("¡El archivo %s no existe!\n", argv[1]);
		return 2;
	}

	/* Leemos las dimensiones a partir del archivo */
	fscanf(input_file, "%d", &dim);

	int matrix[dim][dim];

	i = 0;
	while (!feof(input_file)) {
		fscanf(input_file, "%s", help);
		token = strtok(help, ",");
		j = 0;
		while(token != NULL) {
			matrix[i][j] = atoi(token);
			if (matrix[i][j] == 0) {
				blank_x = j;
				blank_y = i;
			}
			token = strtok(NULL, ",");
			j++;
		}
		i++;
	}
	fclose(input_file);
	displayMatrix(matrix);

	///////////////////////////////////////////
	////////// FIN LECTURA ARCHIVO ////////////
	///////////////////////////////////////////

	HashTable hashTable;
  hashTable = HashTableCreate();
	node_t *endNode = bfs(hashTable, matrix, blank_x, blank_y);
	if (endNode != NULL) {
		node_t *i = endNode;
		i -> child = NULL;
		while (i -> parent != NULL) {
			i -> parent -> child = i;
			i = i -> parent;
		}
		i = i -> child;
		int steps = 0;
		FILE* output_file = fopen("result.txt", "w");
		while (i != NULL) {
			printf("%d,%d\n", i -> blank_x, i -> blank_y);
			fprintf(output_file, "%d,%d\n", i -> blank_x, i -> blank_y);
			steps++;
			i = i -> child;
		}
		fclose(output_file);
		printf("STEPS: %d\n", steps);
	}
	return 0;
}
