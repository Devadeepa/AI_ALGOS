#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE_SIZE 1000

// State structure
typedef struct {
    int cannibalsLeft;
    int missionariesLeft;
    int cannibalsRight;
    int missionariesRight;
    int boat; // 0 for left side, 1 for right side
    int parent; // Index of parent state in the queue
} State;

// Queue structure
typedef struct {
    State states[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// Initialize a queue
void initQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}

// Check if the queue is empty
int isEmpty(Queue* q) {
    return q->front == q->rear;
}

// Enqueue a state
void enqueue(Queue* q, State state) {
    q->states[q->rear++] = state;
}

// Dequeue a state
State dequeue(Queue* q) {
    return q->states[q->front++];
}

// Check if a state is valid
int isValid(State state) {
    if (state.cannibalsLeft < 0 || state.missionariesLeft < 0 ||
        state.cannibalsRight < 0 || state.missionariesRight < 0)
        return 0;

    if ((state.missionariesLeft > 0 && state.cannibalsLeft > state.missionariesLeft) ||
        (state.missionariesRight > 0 && state.cannibalsRight > state.missionariesRight))
        return 0;

    return 1;
}

// Check if two states are equal
int isEqual(State a, State b) {
    return a.cannibalsLeft == b.cannibalsLeft && a.missionariesLeft == b.missionariesLeft &&
           a.cannibalsRight == b.cannibalsRight && a.missionariesRight == b.missionariesRight &&
           a.boat == b.boat;
}

// Check if a state is in the queue
int isInQueue(Queue* q, State state) {
    for (int i = q->front; i < q->rear; i++) {
        if (isEqual(q->states[i], state))
            return 1;
    }
    return 0;
}

// Print the solution
void printSolution(Queue* q, int index) {
    if (index == -1)
        return;
    printSolution(q, q->states[index].parent);
    printf("(%d, %d, %d, %d, %d)\n", q->states[index].cannibalsLeft, q->states[index].missionariesLeft,
           q->states[index].cannibalsRight, q->states[index].missionariesRight, q->states[index].boat);
}

// Solve the cannibals and missionaries problem using BFS
void solve() {
    Queue q;
    initQueue(&q);

    State initialState = {3, 3, 0, 0, 0, -1};
    enqueue(&q, initialState);

    State goalState = {0, 0, 3, 3, 1, -1};

    while (!isEmpty(&q)) {
        State currentState = dequeue(&q);

        if (isEqual(currentState, goalState)) {
            printf("Solution found:\n");
            printSolution(&q, q.front - 1);
            return;
        }

        State nextStates[5];
        int n = 0;

        // Generate possible next states
        if (currentState.boat == 0) { // Boat on the left side
            nextStates[n++] = (State){currentState.cannibalsLeft - 2, currentState.missionariesLeft,
                                      currentState.cannibalsRight + 2, currentState.missionariesRight, 1,
                                      q.front - 1};
            nextStates[n++] = (State){currentState.cannibalsLeft, currentState.missionariesLeft - 2,
                                      currentState.cannibalsRight, currentState.missionariesRight + 2, 1,
                                      q.front - 1};
            nextStates[n++] = (State){currentState.cannibalsLeft - 1, currentState.missionariesLeft - 1,
                                      currentState.cannibalsRight + 1, currentState.missionariesRight + 1, 1,
                                      q.front - 1};
            nextStates[n++] = (State){currentState.cannibalsLeft - 1, currentState.missionariesLeft,
                                      currentState.cannibalsRight + 1, currentState.missionariesRight, 1,
                                      q.front - 1};
            nextStates[n++] = (State){currentState.cannibalsLeft, currentState.missionariesLeft - 1,
                                      currentState.cannibalsRight, currentState.missionariesRight + 1, 1,
                                      q.front - 1};
        } else { // Boat on the right side
            nextStates[n++] = (State){currentState.cannibalsLeft + 2, currentState.missionariesLeft,
                                      currentState.cannibalsRight - 2, currentState.missionariesRight, 0,
                                      q.front - 1};
            nextStates[n++] = (State){currentState.cannibalsLeft, currentState.missionariesLeft + 2,
                                      currentState.cannibalsRight, currentState.missionariesRight - 2, 0,
                                      q.front - 1};
            nextStates[n++] = (State){currentState.cannibalsLeft + 1, currentState.missionariesLeft + 1,
                                      currentState.cannibalsRight - 1, currentState.missionariesRight - 1, 0,
                                      q.front - 1};
            nextStates[n++] = (State){currentState.cannibalsLeft + 1, currentState.missionariesLeft,
                                      currentState.cannibalsRight - 1, currentState.missionariesRight, 0,
                                      q.front - 1};
            nextStates[n++] = (State){currentState.cannibalsLeft, currentState.missionariesLeft + 1,
                                      currentState.cannibalsRight, currentState.missionariesRight - 1, 0,
                                      q.front - 1};
        }

        // Enqueue valid next states
        for (int i = 0; i < n; i++) {
            if (isValid(nextStates[i]) && !isInQueue(&q, nextStates[i])) {
                enqueue(&q, nextStates[i]);
            }
        }
    }

    printf("No solution found.\n");
}

int main() {
    solve();
    return 0;
}
