#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "dict.h"
#include "shared.h"

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

node_t* bfs(HashTable hashTable, int matrix[dim][dim], int blank_x, int blank_y);

void matrixToString(char state[], int matrix[dim][dim]);

int generateNewState(char *newState, int blank_x, int blank_y, int operation);

void append(queue *queueList, node_t *newNode);
