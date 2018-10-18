#include "bfs.h"

/* Transform Matrix state to string state */
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

/* Generate new state from string state */
int generateNewState(char *newState, int blank_x, int blank_y, int operation){
	int extra_step = 0; /* To control Toro restriction */
	if (operation == 0) { /* UP */
		if (!(blank_y > 0)){
			extra_step = dim;
		}
		char moving_block = newState[(blank_y - (1 - extra_step)) * dim + blank_x];
		newState[(blank_y - (1 - extra_step)) * dim + blank_x] = '0';
		newState[blank_y * dim + blank_x] = moving_block;
	}
	else if (operation == 1) { /* LEFT */
		if (!(blank_x > 0)) {
			extra_step = dim;
		}
		char moving_block = newState[blank_y * dim + (blank_x - (1 - extra_step))];
		newState[blank_y * dim + (blank_x - (1 - extra_step))] = '0';
		newState[blank_y * dim + blank_x] = moving_block;

	}
	else if (operation == 2) { /* DOWN */
		if (!(blank_y < dim - 1)) {
			extra_step = dim;
		}
		char moving_block = newState[(blank_y + (1 - extra_step)) * dim + blank_x];
		newState[(blank_y + (1 - extra_step)) * dim + blank_x] = '0';
		newState[blank_y * dim + blank_x] = moving_block;
	}
	else if (operation == 3) { /* RIGHT */
		if (!(blank_x < dim - 1)) {
			extra_step = dim;
		}
		char moving_block = newState[blank_y * dim + (blank_x + (1 - extra_step))];
		newState[blank_y * dim + (blank_x + (1 - extra_step))] = '0';
		newState[blank_y * dim + blank_x] = moving_block;
	}
	return extra_step;
}

/* Add new state to queue */
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

/* Get first state from queue */
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

/* Breadth first search algorithm */
node_t* bfs(HashTable hashTable, int matrix[dim][dim], int blank_x, int blank_y)
{
	queue *open = malloc(sizeof(queue));
	open -> size = 0;
	char initState[dim * dim + 1];
	matrixToString(initState, matrix);
	node_t *startingNode = malloc(sizeof(node_t));
	startingNode -> parent = NULL;
	startingNode -> blank_x = blank_x;
	startingNode -> blank_y = blank_y;
	startingNode -> state = strdup(initState);
	append(open, startingNode);
	HashTableInsert(hashTable, initState);

	while (open -> size > 0)
	{
		node_t *s = popLeft(open);

		for (int i = 0; i < 4; ++i)
		{
			char *newState = strdup(s -> state);
			int extra_step = generateNewState(newState, s -> blank_x, s -> blank_y, i);
			if (HashTableSearch(hashTable, newState) == true){
				continue;
			}
			int new_blank_x = s -> blank_x;
			int new_blank_y = s -> blank_y;

			if (i == 0)
				new_blank_y -= (1 - extra_step);
			else if (i == 1)
				new_blank_x -= (1 - extra_step);
			else if (i == 2)
				new_blank_y += (1 - extra_step);
			else if (i == 3)
				new_blank_x += (1 - extra_step);
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
			HashTableInsert(hashTable, newState);

		}
	}
	return NULL;
}
