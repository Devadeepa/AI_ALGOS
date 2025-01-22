#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 3
#define MAX_HEAP_SIZE 100000

typedef struct {
    int tiles[N][N];
    int g; // Cost from start to current node
    int h; // Heuristic cost to goal
    int f; // Total cost (g + h)
    int empty_row, empty_col; // Position of the empty tile
} Node;

typedef struct {
    Node nodes[MAX_HEAP_SIZE];
    int size;
} MinHeap;

// Function to create a new node
Node createNode(int tiles[N][N], int g, int empty_row, int empty_col) {
    Node node;
    memcpy(node.tiles, tiles, sizeof(node.tiles));
    node.g = g;
    node.empty_row = empty_row;
    node.empty_col = empty_col;
    node.h = 0; // Initialize h to 0
    node.f = 0; // Initialize f to 0
    return node;
}

// Function to calculate the Manhattan distance heuristic
int calculateHeuristic(int tiles[N][N]) {
    int h = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int value = tiles[i][j];
            if (value != 0) {
                int target_row = (value - 1) / N;
                int target_col = (value - 1) % N;
                h += abs(i - target_row) + abs(j - target_col);
            }
        }
    }
    return h;
}

// Function to compare two nodes based on their total cost (f)
int compareNodes(const Node* a, const Node* b) {
    return a->f - b->f;
}

// Function to initialize a min-heap
void initMinHeap(MinHeap* heap) {
    heap->size = 0;
}

// Function to insert a node into the min-heap
void insertMinHeap(MinHeap* heap, Node node) {
    int i = heap->size++;
    while (i > 0 && compareNodes(&node, &heap->nodes[(i - 1) / 2]) < 0) {
        heap->nodes[i] = heap->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->nodes[i] = node;
}

// Function to extract the minimum node from the min-heap
Node extractMinHeap(MinHeap* heap) {
    Node minNode = heap->nodes[0];
    Node lastNode = heap->nodes[--heap->size];
    int i = 0, child;
    while ((child = 2 * i + 1) < heap->size) {
        if (child + 1 < heap->size && compareNodes(&heap->nodes[child + 1], &heap->nodes[child]) < 0) {
            ++child;
        }
        if (compareNodes(&lastNode, &heap->nodes[child]) <= 0) {
            break;
        }
        heap->nodes[i] = heap->nodes[child];
        i = child;
    }
    heap->nodes[i] = lastNode;
    return minNode;
}

// Function to check if two states are equal
int isEqual(int tiles1[N][N], int tiles2[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (tiles1[i][j] != tiles2[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

// Function to solve the 8-puzzle problem using the A* algorithm
void solvePuzzle(int start_tiles[N][N], int goal_tiles[N][N]) {
    MinHeap openList;
    initMinHeap(&openList);

    // Find the empty tile position in the start state
    int empty_row, empty_col;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (start_tiles[i][j] == 0) {
                empty_row = i;
                empty_col = j;
                break;
            }
        }
    }

    int start_h = calculateHeuristic(start_tiles);
    Node startNode = createNode(start_tiles, 0, empty_row, empty_col);
    startNode.h = start_h;
    startNode.f = startNode.g + startNode.h;
    insertMinHeap(&openList, startNode);

    while (openList.size > 0) {
        Node currentNode = extractMinHeap(&openList);

        if (isEqual(currentNode.tiles, goal_tiles)) {
            printf("Puzzle solved in %d moves!\n", currentNode.g);
            return;
        }

        // Generate possible moves (up, down, left, right)
        int row_moves[] = {-1, 1, 0, 0};
        int col_moves[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; ++i) {
            int new_row = currentNode.empty_row + row_moves[i];
            int new_col = currentNode.empty_col + col_moves[i];

            if (new_row >= 0 && new_row < N && new_col >= 0 && new_col < N) {
                Node newNode = currentNode;
                newNode.tiles[currentNode.empty_row][currentNode.empty_col] = newNode.tiles[new_row][new_col];
                newNode.tiles[new_row][new_col] = 0;
                newNode.empty_row = new_row;
                newNode.empty_col = new_col;
                newNode.g = currentNode.g + 1;
                newNode.h = calculateHeuristic(newNode.tiles);
                newNode.f = newNode.g + newNode.h;

                insertMinHeap(&openList, newNode);
            }
        }
    }

    printf("Puzzle is unsolvable.\n");
}

// Function to print the puzzle tiles
void printPuzzle(int tiles[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%2d ", tiles[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int start_tiles[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 0, 8}
    };

    int goal_tiles[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    printf("Start state:\n");
    printPuzzle(start_tiles);

    printf("Goal state:\n");
    printPuzzle(goal_tiles);

    solvePuzzle(start_tiles, goal_tiles);

    return 0;
}
