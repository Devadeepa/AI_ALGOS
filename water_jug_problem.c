#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_SIZE 1000
typedef struct State {
    int jug1, jug2;
    int f, g, h;
    struct State* parent;
} State;
typedef struct PriorityQueue {
    State* states[MAX_SIZE];
    int size;
} PriorityQueue;
State* createState(int j1, int j2, int g, int target, State* p) {
    State* s = (State*)malloc(sizeof(State));
    s->jug1 = j1;
    s->jug2 = j2;
    s->g = g;
    s->h = abs(j1 + j2 - target);
    s->f = s->g + s->h;
    s->parent = p;
    return s;
}
void insertPQ(PriorityQueue* pq, State* s) {
    int i = pq->size++;
    while (i > 0 && pq->states[(i-1)/2]->f > s->f) {
        pq->states[i] = pq->states[(i-1)/2];
        i = (i-1)/2;
    }
    pq->states[i] = s;
}
State* removePQ(PriorityQueue* pq) {
    State* min = pq->states[0];
    State* last = pq->states[--pq->size];
    int i = 0, child;
    
    while ((child = 2*i + 1) < pq->size) {
        if (child + 1 < pq->size && pq->states[child+1]->f < pq->states[child]->f)
            child++;
        if (last->f <= pq->states[child]->f) break;
        pq->states[i] = pq->states[child];
        i = child;
    }
    pq->states[i] = last;
    return min;
}
bool isVisited(State** visited, int visitedSize, State* s) {
    for (int i = 0; i < visitedSize; i++) {
        if (visited[i]->jug1 == s->jug1 && visited[i]->jug2 == s->jug2)
            return true;
    }
    return false;
}
void printPath(State* s) {
    if (!s) return;
    printPath(s->parent);
    printf("(%d, %d)\n", s->jug1, s->jug2);
}
void solvePuzzle(int capacity1, int capacity2, int target) {
   PriorityQueue pq;
pq.size = 0;
    State* visited[MAX_SIZE];
    int visitedSize = 0;
    
    State* initial = createState(0, 0, 0, target, NULL);
    insertPQ(&pq, initial);
    
    while (pq.size > 0) {
        State* current = removePQ(&pq);
        
        if (current->jug1 + current->jug2 == target) {
            printf("Solution found!\nPath:\n");
            printPath(current);
            return;
        }
        
        if (isVisited(visited, visitedSize, current))
            continue;
            
        visited[visitedSize++] = current;
        
        // Fill jug1
        if (current->jug1 < capacity1) {
            State* next = createState(capacity1, current->jug2, current->g + 1, target, current);
            if (!isVisited(visited, visitedSize, next))
                insertPQ(&pq, next);
        }
        
        // Fill jug2
        if (current->jug2 < capacity2) {
            State* next = createState(current->jug1, capacity2, current->g + 1, target, current);
            if (!isVisited(visited, visitedSize, next))
                insertPQ(&pq, next);
        }
        
        // Empty jug1
        if (current->jug1 > 0) {
            State* next = createState(0, current->jug2, current->g + 1, target, current);
            if (!isVisited(visited, visitedSize, next))
                insertPQ(&pq, next);
        }
        
        // Empty jug2
        if (current->jug2 > 0) {
            State* next = createState(current->jug1, 0, current->g + 1, target, current);
            if (!isVisited(visited, visitedSize, next))
                insertPQ(&pq, next);
        }
        
        // Pour jug1 to jug2
        if (current->jug1 > 0 && current->jug2 < capacity2) {
            int amount = (current->jug1 + current->jug2 <= capacity2) ? 
                        current->jug1 : capacity2 - current->jug2;
            State* next = createState(current->jug1 - amount, 
                                    current->jug2 + amount, 
                                    current->g + 1, target, current);
            if (!isVisited(visited, visitedSize, next))
                insertPQ(&pq, next);
        }
        
        // Pour jug2 to jug1
        if (current->jug2 > 0 && current->jug1 < capacity1) {
            int amount = (current->jug1 + current->jug2 <= capacity1) ? 
                        current->jug2 : capacity1 - current->jug1;
            State* next = createState(current->jug1 + amount, 
                                    current->jug2 - amount, 
                                    current->g + 1, target, current);
            if (!isVisited(visited, visitedSize, next))
                insertPQ(&pq, next);
        }
    }
    
    printf("No solution found!\n");
}
int main() {
    int jug1_capacity = 4;
    int jug2_capacity = 3;
    int target = 2;
    
    solvePuzzle(jug1_capacity, jug2_capacity, target);
    return 0;
}
