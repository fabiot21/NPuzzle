#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "random.h"
#include "dict.h"

/* Las dimensiones del problema */
int dim;
int states = 0;

int blank_x = -1;
int blank_y = -1;

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

void matrixToString(char state[], int matrix[dim][dim])
{
	int index = 0;
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			state[index] = matrix[i][j] + '0';
			index++;
		}
	}
	state[index] = '\0';
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

bool generateNewState(char *newState, int blank_x, int blank_y, int operation){
	if (operation == 0) { /* UP */
		if (blank_y > 0) {
			char moving_block = newState[(blank_y - 1) * dim + blank_x];
			newState[(blank_y - 1) * dim + blank_x] = '0';
			newState[blank_y * dim + blank_x] = moving_block;
		} else {
			return false;
		}
	}
	else if (operation == 1) { /* LEFT */
		if (blank_x > 0) {
			char moving_block = newState[blank_y * dim + (blank_x - 1)];
			newState[blank_y * dim + (blank_x - 1)] = '0';
			newState[blank_y * dim + blank_x] = moving_block;
		} else {
			return false;
		}

	}
	else if (operation == 2) { /* DOWN */
		if (blank_y < dim - 1) {
			char moving_block = newState[(blank_y + 1) * dim + blank_x];
			newState[(blank_y + 1) * dim + blank_x] = '0';
			newState[blank_y * dim + blank_x] = moving_block;
		} else {
			return false;
		}
	}
	else if (operation == 3) { /* RIGHT */
		if (blank_x < dim - 1) {
			char moving_block = newState[blank_y * dim + (blank_x + 1)];
			newState[blank_y * dim + (blank_x + 1)] = '0';
			newState[blank_y * dim + blank_x] = moving_block;
		} else {
			return false;
		}
	}
	return true;
}

typedef struct node
{
    char *state;
    int blank_x;
    int blank_y;
    struct node *parent;
    struct node *next;
    struct node *child;
} node_t;

typedef struct queue
{
	node_t *first;
	node_t *last;
	int size;
} queue;


void append(queue *queueList, node_t *newNode)
{
	newNode -> next = NULL;
	if (queueList -> size == 0)
	{
		queueList -> last = newNode;
		queueList -> first = newNode;
	}
	else
	{
		queueList -> last -> next = newNode;
		queueList -> last = newNode;
	}
	queueList -> size++;
}

node_t* popLeft(queue *queueList)
{
	node_t *targetNode = queueList -> first;
	if (queueList -> size == 1)
	{
		queueList -> first = NULL;
		queueList -> last = NULL;
		queueList -> size --;
		return targetNode;
	}
	queueList -> first = targetNode -> next;
	queueList -> size--;
	return targetNode;
}

node_t* bfs(Dict dict, int matrix[dim][dim], int blank_x, int blank_y)
{
	queue *open = malloc(sizeof(queue));
	open -> size = 0;
	char initState[dim*dim+1];
	matrixToString(initState, matrix);
	node_t *startingNode = malloc(sizeof(node_t));
	startingNode -> parent = NULL;
	startingNode -> blank_x = blank_x;
	startingNode -> blank_y = blank_y;
	startingNode -> state = strdup(initState);
	append(open, startingNode);
	DictInsert(dict, initState);

	while (open -> size > 0)
	{
		node_t *s = popLeft(open);
		
		for (int i = 0; i < 4; ++i)
		{		
			char *newState = strdup(s -> state);
			printf("%d\n", states);
			bool moved = generateNewState(newState, s -> blank_x, s -> blank_y, i);
			states++;
			if (!moved){
				continue;
			}
			if (DictSearch(dict, newState) == true){
				continue;
			}
			int new_blank_x = s -> blank_x;
			int new_blank_y = s -> blank_y;
			if (i == 0)
				new_blank_y -= 1;
			else if (i == 1)
				new_blank_x -= 1;
			else if (i == 2)
				new_blank_y += 1;
			else if (i == 3)
				new_blank_x += 1; 
			node_t *t = malloc(sizeof(node_t));
			t -> parent = s;
			t -> blank_x = new_blank_x;
			t -> blank_y = new_blank_y;
			t -> state = newState;
			/* Verify Goal */
			if (newState[dim*dim - 1] == '0' && newState[0] == '1') {
				bool end = true;
				char n = '1';
				for (int i = 0; i < (dim * dim) - 1; ++i)
				{
					if (newState[i] != n){
						end = false;
						break;
					}
					n++;
				}
				if (end)
					return t;
			}
			append(open, t);
			DictInsert(dict, newState);
			
		}	
	}
	return NULL;
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
	displayMatrix(matrix);

	Dict dict;
    dict = DictCreate();
	node_t *endNode = bfs(dict, matrix, blank_x, blank_y);
	if (endNode != NULL) {
		printf("%s\n", endNode -> state);
	}

	///////////////////////////////////////////
	////////// FIN LECTURA ARCHIVO ////////////
	///////////////////////////////////////////

	return 0;
}
