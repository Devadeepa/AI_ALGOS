#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 5
#define COLS 5

typedef struct {
    int row, col;
} Position;

typedef struct Node {
    Position pos;
    int g, h;
    struct Node* parent;
} Node;

typedef struct {
    Node* nodes[ROWS * COLS];
    int size;
} PriorityQueue;

PriorityQueue* createPriorityQueue() {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->size = 0;
    return pq;
}

void push(PriorityQueue* pq, Node* node) {
    pq->nodes[pq->size++] = node;
}

Node* pop(PriorityQueue* pq) {
    int bestIndex = 0;
    for (int i = 1; i < pq->size; ++i) {
        if (pq->nodes[i]->g + pq->nodes[i]->h < pq->nodes[bestIndex]->g + pq->nodes[bestIndex]->h) {
            bestIndex = i;
        }
    }
    Node* bestNode = pq->nodes[bestIndex];
    pq->nodes[bestIndex] = pq->nodes[--pq->size];
    return bestNode;
}

int isEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

int heuristic(Position a, Position b) {
    return abs(a.row - b.row) + abs(a.col - b.col);
}

int isValid(Position pos) {
    return pos.row >= 0 && pos.row < ROWS && pos.col >= 0 && pos.col < COLS;
}

int isEqual(Position a, Position b) {
    return a.row == b.row && a.col == b.col;
}

void printPath(Node* node) {
    if (node == NULL) return;
    printPath(node->parent);
    printf("(%d, %d) -> ", node->pos.row, node->pos.col);
}

void solveAStar(Position start, Position goal) {
    PriorityQueue* openList = createPriorityQueue();
    int closedList[ROWS][COLS] = {0};

    Node* startNode = (Node*)malloc(sizeof(Node));
    startNode->pos = start;
    startNode->g = 0;
    startNode->h = heuristic(start, goal);
    startNode->parent = NULL;
    push(openList, startNode);

    Position directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while (!isEmpty(openList)) {
        Node* current = pop(openList);
        if (isEqual(current->pos, goal)) {
            printf("Path: ");
            printPath(current);
            printf("Goal\n");
            return;
        }

        closedList[current->pos.row][current->pos.col] = 1;

        for (int i = 0; i < 4; ++i) {
            Position newPos = {current->pos.row + directions[i].row, current->pos.col + directions[i].col};
            if (isValid(newPos) && !closedList[newPos.row][newPos.col]) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->pos = newPos;
                newNode->g = current->g + 1;
                newNode->h = heuristic(newPos, goal);
                newNode->parent = current;
                push(openList, newNode);
            }
        }
    }

    printf("No path found!\n");
}

int main() {
    Position start = {0, 0};
    Position goal = {4, 4};
    solveAStar(start, goal);
    return 0;
}
