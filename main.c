//
//  main.c
//  binaryTree
//
//  Created by Alex Owen-Meehan on 17/11/2016.
//  Copyright © 2016 Alex Owen-Meehan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "binaryTree.h"
#include "queue.h"

#define MAXLENGTH 20

void initNode(char *data, Node **leaf) {
	*leaf = (Node*)malloc(sizeof(Node));
	(*leaf)->data = data;
	(*leaf)->left = 0;
	(*leaf)->right = 0;
}

void insertAtPoint(char *data, Node **root, char *parent, int isChild) {
	Node *traverse;

	if (*root == 0) {
		initNode(data, root);
		return;
	}

	push(*root);

	while (!isEmpty()) {
		traverse = pop();

		if (strcmp(traverse->data, parent) == 0) {
			if (isChild) {
				if (traverse->left != 0) {
					insertAtPoint(data, root, (traverse->left)->data, 0);
				} else {
					initNode(data, &(traverse)->left);
					resetQueue();
					return;
				}
			} else {
				if (traverse->right != 0) {
					insertAtPoint(data, root, (traverse->right)->data, 0);
				} else {
					initNode(data, &(traverse)->right);
					resetQueue();
					return;
				}
			}

			resetQueue();
			return;
		}

		if (traverse->left != 0) {
			push(traverse->left);
		}

		if (traverse->right != 0) {
			push(traverse->right);
		}
	}

	resetQueue();
	return;
}

void insert(char *data, Node **root){

	Node *traverse;

	if (*root == 0) { /* If root doesn't exist */
		initNode(data, root);
		return;	
	}

	push(*root);

	while (!isEmpty()) {
		traverse = pop();

		if (traverse->left != 0) {
			push(traverse->left);
		} else {
			initNode(data, &(traverse)->left);
			resetQueue();
			return;
		}

		if (traverse->right != 0) {
			push(traverse->right);
		} else {
			initNode(data, &(traverse)->right);
			resetQueue();
			return;
		}
	}
}

int getSize(Node *root) {
	Node *traverse;
	int count = 0;

	if (root == 0) {
		return 0;
	}

	push(root);

	while (!isEmpty()) {
		traverse = pop();
		++count;

		if (traverse->left != 0) {
			push(traverse->left);
		}
		if (traverse->right != 0) {
			push(traverse->right);
		}
	}

	return count;
}

void destroyTree(Node *node) {
	if (node != 0) {
		destroyTree(node->left);
		destroyTree(node->right);
		free(node);
	}
}

int getDepth(Node *root) {
	if (root == 0) {
		return 0;
	}

	int leftDepth = getDepth(root->left);
	int rightDepth = getDepth(root->right);
	return leftDepth < rightDepth ? leftDepth + 1 : rightDepth + 1;
}

int getCountAtLevel(Node *node, int currentDepth, int desiredDepth) {
	if (node == 0) {
		return 0;
	}
	if (currentDepth == desiredDepth) {
		return 1;
	}

	int count = 0;
	count += getCountAtLevel(node->left, currentDepth + 1, desiredDepth);
	count += getCountAtLevel(node->right, currentDepth + 1, desiredDepth);
	return count;
}

void getLevels(Node *root, int **levels) {
	int maxDepth = getDepth(root);
	int i;

	if (root == 0) {
		return;
	}

	*levels = (int*)calloc(maxDepth, sizeof(int));
	for (i = 0; i <= maxDepth; i++) {
		(*levels)[i] = getCountAtLevel(root, 0, i);
	}
}

void centreString(char **str) {
	char *temp;
	int len = strlen(*str);
	int i;

	temp = (char*)malloc((len + 1) * sizeof(char));
	strcpy(temp, *str);
	memset(*str, 0, len);
	for (i = 0; i < (MAXLENGTH - len) / 2; i++) {
		(*str)[i] = ' ';
	}
	strcat(*str, temp);
	i += len;
	for (; i <= MAXLENGTH; i++) {
		(*str)[i] = ' ';
	}
	(*str)[MAXLENGTH] = '\0';
}

int printNode(Node *node, int isLeft, int offset, int depth, char **output) {
	int i;
	char *b;

	if (node == 0) {
		return 0;
	}

	b = (char*)malloc(MAXLENGTH * sizeof(char));
	strcpy(b, node->data);

	centreString(&b);

	int left = printNode(node->left, 1, offset, depth + 1, output);
	int right = printNode(node->right, 0, offset + left + MAXLENGTH, depth + 1, output);

	for (i = 0; i < MAXLENGTH; i++) {
		output[depth][offset + left + i] = b[i];
	}

	if (depth && isLeft) {
		for (i = 0; i < MAXLENGTH + right; i++) {
			output[depth - 1][offset + left + (MAXLENGTH / 2) + i] = '-';
		}

		output[depth - 1][offset + left + (MAXLENGTH / 2)] = '.';
	} else if (depth && !isLeft) {
		for (i = 0; i < left + MAXLENGTH; i++) {
			output[depth - 1][offset - (MAXLENGTH / 2) + i] = '-';
		}

		output[depth - 1][offset + left + (MAXLENGTH / 2)] = '.';
	}

	return left + MAXLENGTH + right;
}

void printBinaryTree(Node *root, int *levels) {
	int i, j, max = 0, width = 0;
	int nameCount = getSize(root);
	int maxDepth = getDepth(root);

	for (i = 0; i <= maxDepth; i++) {
		if (levels[i] > max) {
			max = levels[i];
		}
	}

	char **output = malloc(maxDepth * sizeof(char*));
	width = ((max * MAXLENGTH) + (max * 2));

	for (i = 0; i <= maxDepth; i++) {
		output[i] = (char*)malloc(width * sizeof(char));
		for (j = 0; j < (width * sizeof(char)); j++) {
			output[i][j] = ' ';
		}
		output[i][(width * sizeof(char))] = '\0';
	}

	printNode(root, 0, 0, 0, output);

	for (i = 0; i <= maxDepth; i++) {
		printf("%s\r\n", output[i]);
	}
}

/*
void printBinaryTree(Node *root){
    
    int currentCount, nextCount, depth, level, isFirst;
	currentCount = level = isFirst = 1;
	nextCount = 0;
	depth = getDepth(root);
    
	Node *traverse;

	if (root == 0) {
		return;
	}

	push(root);

	while (!isEmpty()) {
		traverse = pop();
		--currentCount;

		if (isFirst) {
			int i;
			for (i = 0; i < pow(2, depth - level); i++) {
				printf("  ");
			}
			isFirst = 0;
		}

		if (traverse != 0) {
			if (traverse->data < 10) {
				printf("%d ", traverse->data);
			} else {
				printf("%d", traverse->data);
			}
			push(traverse->left);
			push(traverse->right);
			nextCount += 2;
		}

		int i;
		for (i = 0; i < 2*pow(2, depth - level + 1) - 2; i++) {
			printf(" ");
		}

		if (currentCount == 0) {
			printf("\r\n");
			currentCount = nextCount;
			nextCount = 0;
			++level;
			isFirst = 1;
		}
	}
	resetQueue();
}
*/
int main(void) {
	char **names;
	int i, count;

	count = 8;

	names = (char**)malloc(count * sizeof(char*));
	for (i = 0; i < count; i++) {
		names[i] = (char*)malloc(MAXLENGTH * sizeof(char));
	}
	strcpy(names[0], "Some Name");
	strcpy(names[1], "John Smith");
	strcpy(names[2], "Harold McMeme");
	strcpy(names[3], "Firstname Lastname");
	strcpy(names[4], "Nomen Cognomen");
	strcpy(names[5], "Adam Godson");
	strcpy(names[6], "Wu Tang");
	strcpy(names[7], "Someone Else");
    
    Node *root = 0;

	insertAtPoint(names[0], &root, "", 0);
	insertAtPoint(names[1], &root, names[0], 1);
	insertAtPoint(names[2], &root, names[0], 0);
	insertAtPoint(names[3], &root, names[0], 1);
	insertAtPoint(names[4], &root, names[1], 1);
	insertAtPoint(names[5], &root, names[1], 0);
	insertAtPoint(names[6], &root, names[2], 1);
	insertAtPoint(names[7], &root, names[2], 0);

	int *levels;
	getLevels(root, &levels);
	

	printBinaryTree(root, levels);

	/*
	char *test = malloc(MAXLENGTH * sizeof(char));
	strcpy(test, "testing");
	printf("%s\r\n", test);
	centreString(&test);
	printf("%s\r\n", test);
	*/

	for (i = 0; i < count; i++) {
		free(names[i]);
	}
	free(names);

	destroyTree(root);
    return 0;
}