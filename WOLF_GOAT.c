#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h> 
typedef struct State { 
 bool farmer; // 0 for left bank, 1 for right bank 
 bool wolf; // 0 for left bank, 1 for right bank 
 bool goat; // 0 for left bank, 1 for right bank 
 bool cabbage; // 0 for left bank, 1 for right bank 
 struct State* parent; // Pointer to parent state 
} State; 
// Function to create a new state 
State* createState(bool farmer, bool wolf, bool goat, bool cabbage, State* parent) { 
 State* newState = (State*)malloc(sizeof(State)); 
 newState->farmer = farmer; 
 newState->wolf = wolf; 
 newState->goat = goat; 
 newState->cabbage = cabbage; 
 newState->parent = parent; 
 return newState; 
} 
// Check if we reached the goal state 
bool isGoalState(State* state) { 
 return state->farmer && state->wolf && state->goat && state->cabbage; 
} 
// Check if the state is valid 
bool isValidState(State* state) { 
 // The wolf can't be left alone with the goat, and the goat can't be left alone with the cabbage 
 if ((state->wolf == state->goat && state->farmer != state->wolf) || 
 (state->goat == state->cabbage && state->farmer != state->goat)) { 
 return false; 
 } 
 return true; 
} 
// Heuristic function (number of items left on the left bank) 
int heuristic(State* state) { 
 return (!state->farmer) + (!state->wolf) + (!state->goat) + (!state->cabbage); 
} 
// Print the solution path 
void printSolution(State* state) { 
 if (state == NULL) return; 
 printSolution(state->parent); 
 printf("Farmer: %s | Wolf: %s | Goat: %s | Cabbage: %s\n", 
 state->farmer ? "Right" : "Left", 
 state->wolf ? "Right" : "Left", 
 state->goat ? "Right" : "Left", 
 state->cabbage ? "Right" : "Left"); 
} 
// Check if the state has already been explored 
bool isExplored(State** explored, int count, State* state) { 
 for (int i = 0; i < count; i++) { 
 if (explored[i]->farmer == state->farmer && 
 explored[i]->wolf == state->wolf && 
 explored[i]->goat == state->goat && 
 explored[i]->cabbage == state->cabbage) { 
 return true; 
 } 
 } 
 return false; 
} 
// Generate possible next states 
void generateStates(State* current, State** nextStates, int* nextCount) { 
 bool farmer = current->farmer; 
 // Generate possible moves 
 // 1. Farmer crosses alone 
 nextStates[(*nextCount)++] = createState(!farmer, current->wolf, current->goat, current->cabbage, current); 
 // 2. Farmer takes the wolf 
 if (current->wolf == farmer) { 
 nextStates[(*nextCount)++] = createState(!farmer, !farmer, current->goat, current->cabbage, current); 
 } 
 // 3. Farmer takes the goat 
 if (current->goat == farmer) { 
 nextStates[(*nextCount)++] = createState(!farmer, current->wolf, !farmer, current->cabbage, current); 
 } 
// 4. Farmer takes the cabbage 
 if (current->cabbage == farmer) { 
 nextStates[(*nextCount)++] = createState(!farmer, current->wolf, current->goat, !farmer, current); 
 } 
} 
void aStarSearch() { 
 State* start = createState(0, 0, 0, 0, NULL); // All on the left bank 
 State* explored[100]; // Keep track of explored states 
 int exploredCount = 0; 
 State* queue[100]; // Simple queue to store states 
 int queueCount = 0; 
 queue[queueCount++] = start; 
while (queueCount > 0) { 
 // Find the state with the lowest cost + heuristic 
 int bestIndex = 0; 
 for (int i = 1; i < queueCount; i++) { 
 if (heuristic(queue[i]) < heuristic(queue[bestIndex])) { 
 bestIndex = i; 
 } 
 } 
 State* current = queue[bestIndex]; 
// Check if we reached the goal 
 if (isGoalState(current)) { 
 printf("Solution Found:\n"); 
 printSolution(current); 
 return; 
 } 
// Remove current node from the queue 
 for (int i = bestIndex; i < queueCount - 1; i++) { 
 queue[i] = queue[i + 1]; 
 } 
 queueCount--; 
// Add current node to explored 
 if (!isExplored(explored, exploredCount, current)) { 
 explored[exploredCount++] = current; 
State* nextStates[5]; // To store next possible states 
 int nextCount = 0; 
 generateStates(current, nextStates, &nextCount); 
 for (int i = 0; i < nextCount; i++) { 
 if (isValidState(nextStates[i]) && !isExplored(explored, exploredCount, nextStates[i])) { 
 queue[queueCount++] = nextStates[i]; 
 } else { 
 free(nextStates[i]); // Avoid memory leak for explored nodes 
 } 
 } 
 } 
 } 
 printf("No solution found\n"); 
} 
int main() { 
 aStarSearch(); 
 return 0; 
}
