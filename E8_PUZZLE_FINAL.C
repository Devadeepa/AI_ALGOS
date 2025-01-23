#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct Node {
    char data[MAX_SIZE][MAX_SIZE];
    int level;
    int fval;
    int size;
} Node;

Node* createNode(char data[MAX_SIZE][MAX_SIZE], int level, int fval, int size) {
    Node* node = (Node*)malloc(sizeof(Node));
    memcpy(node->data, data, sizeof(char) * MAX_SIZE * MAX_SIZE);
    node->level = level;
    node->fval = fval;
    node->size = size;
    return node;
}

void findBlankSpace(char data[MAX_SIZE][MAX_SIZE], int size, int* x, int* y) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (data[i][j] == '_') {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

Node** generateChild(Node* parent) {
    int x, y;
    findBlankSpace(parent->data, parent->size, &x, &y);

    int val_list[4][2] = {{x, y-1}, {x, y+1}, {x-1, y}, {x+1, y}};
    Node** children = (Node**)malloc(sizeof(Node) * 4);
    int childCount = 0;

    for (int i = 0; i < 4; i++) {
        int newX = val_list[i][0];
        int newY = val_list[i][1];

        if (newX >= 0 && newX < parent->size && newY >= 0 && newY < parent->size) {
            char newData[MAX_SIZE][MAX_SIZE];
            memcpy(newData, parent->data, sizeof(char) * MAX_SIZE * MAX_SIZE);

            char temp = newData[newX][newY];
            newData[newX][newY] = newData[x][y];
            newData[x][y] = temp;

            children[childCount++] = createNode(newData, parent->level + 1, 0, parent->size);
        }
    }

    children[childCount] = NULL;
    return children;
}

typedef struct Puzzle {
    int size;
    Node** open;
    Node** closed;
    int openCount;
    int closedCount;
} Puzzle;

Puzzle* createPuzzle(int size) {
    Puzzle* puzzle = (Puzzle*)malloc(sizeof(Puzzle));
    puzzle->size = size;
    puzzle->open = (Node**)malloc(sizeof(Node) * MAX_SIZE * MAX_SIZE);
    puzzle->closed = (Node**)malloc(sizeof(Node) * MAX_SIZE * MAX_SIZE);
    puzzle->openCount = 0;
    puzzle->closedCount = 0;
    return puzzle;
}

void acceptPuzzle(char puzzle[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            scanf(" %c", &puzzle[i][j]);
        }
    }
}

int heuristic(char start[MAX_SIZE][MAX_SIZE], char goal[MAX_SIZE][MAX_SIZE], int size) {
    int temp = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (start[i][j] != goal[i][j] && start[i][j] != '_') {
                temp++;
            }
        }
    }
    return temp;
}

int f(Node* start, char goal[MAX_SIZE][MAX_SIZE], int size) {
    return heuristic(start->data, goal, size) + start->level;
}

void printPuzzle(char data[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%c ", data[i][j]);
        }
        printf("\n");
    }
}

int compare(const void* a, const void* b) {
    return ((Node*)a)->fval - ((Node*)b)->fval;
}

void process(Puzzle* puzzle) {
    char start[MAX_SIZE][MAX_SIZE], goal[MAX_SIZE][MAX_SIZE];

    printf("Enter the start state matrix \n");
    acceptPuzzle(start, puzzle->size);

    printf("Enter the goal state matrix \n");
    acceptPuzzle(goal, puzzle->size);

    Node* startNode = createNode(start, 0, 0, puzzle->size);
    startNode->fval = f(startNode, goal, puzzle->size);

    puzzle->open[puzzle->openCount++] = startNode;

    printf("\n\n");

    while (1) {
        Node* cur = puzzle->open[0];

        printf("\n  | \n");
        printf("  | \n");
        printf(" \\\'/ \n\n");
        printPuzzle(cur->data, puzzle->size);

        if (heuristic(cur->data, goal, puzzle->size) == 0) {
            break;
        }

        Node** children = generateChild(cur);
        for (int i = 0; children[i] != NULL; i++) {
            children[i]->fval = f(children[i], goal, puzzle->size);
            puzzle->open[puzzle->openCount++] = children[i];
        }
        free(children);

        puzzle->closed[puzzle->closedCount++] = cur;

        for (int i = 0; i < puzzle->openCount - 1; i++) {
            puzzle->open[i] = puzzle->open[i + 1];
        }
        puzzle->openCount--;

        qsort(puzzle->open, puzzle->openCount, sizeof(Node*), compare);
    }
}

int main() {
    int size;
    printf("Enter the puzzle size: ");
    scanf("%d", &size);

    Puzzle* puzzle = createPuzzle(size);
    process(puzzle);

    // Free allocated memory
    for (int i = 0; i < puzzle->openCount; i++) {
        free(puzzle->open[i]);
    }
    for (int i = 0; i < puzzle->closedCount; i++) {
        free(puzzle->closed[i]);
    }
    free(puzzle->open);
    free(puzzle->closed);
    free(puzzle);

    return 0;
}
