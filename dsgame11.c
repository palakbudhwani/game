#include <stdio.h>
#include <stdlib.h>
#define SIZE 3

// Stack structure for undo functionality
typedef struct StackNode {
    int row, col;
    struct StackNode* next;
} StackNode;

typedef struct Stack {
    StackNode* top;
} Stack;

// Queue structure for move history
typedef struct QueueNode {
    char board[SIZE][SIZE];
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Function declarations
void printBoard(char board[SIZE][SIZE]);
void resetBoard(char board[SIZE][SIZE]);
void push(Stack* stack, int row, int col);
StackNode* pop(Stack* stack);
void enqueue(Queue* queue, char board[SIZE][SIZE]);

int main() {
    char board[SIZE][SIZE];
    resetBoard(board);
    Stack undoStack = {NULL};
    Queue historyQueue = {NULL, NULL};
    int player = 1, row, col; 
    char mark;

    while (1) {
        printBoard(board);
        mark = (player == 1) ? 'X' : 'O';
        printf("Player %d (%c), enter row and column (0, 1, or 2) or -1 to undo: ", player, mark); 
        scanf("%d %d", &row, &col);

        if (row == -1) {
            StackNode* lastMove = pop(&undoStack); 
            if (lastMove) {
                board[lastMove->row][lastMove->col] = ' ';
                player = (player == 1) ? 2 : 1; // Switch player after undo
                free(lastMove);
                continue;
            }
        }

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != ' ') {
            printf("Invalid move! Try again.\n");
            continue;
        }
        
        board[row][col] = mark; 
        push(&undoStack, row, col); 
        enqueue(&historyQueue, board);

        // Check win conditions
        for (int i = 0; i < SIZE; i++) {
            if ((board[i][0] == mark && board[i][1] == mark && board[i][2] == mark) || 
                (board[0][i] == mark && board[1][i] == mark && board[2][i] == mark)) {
                printBoard(board);
                printf("Player %d (%c) wins!\n", player, mark);
                return 0;
            }
        }
        
        if (board[0][0] == mark && board[1][1] == mark && board[2][2] == mark || 
            board[0][2] == mark && board[1][1] == mark && board[2][0] == mark) {
            printBoard(board);
            printf("Player %d (%c) wins!\n", player, mark);
            return 0;
        }

        // Check for draw
        int full = 1;
        for (int i = 0; i < SIZE && full; i++)
            for (int j = 0; j < SIZE; j++)
                if (board[i][j] == ' ') full = 0;

        if (full) {
            printBoard(board);
            printf("It's a draw!\n");
            return 0;
        }
        
        player = (player == 1) ? 2 : 1; // Switch player
    }
}

// Print the game board
void printBoard(char board[SIZE][SIZE]) {
    printf("\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf(" %c ", board[i][j]);
            if (j < SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < SIZE - 1) printf("---|---|---\n");
    }
    printf("\n");
}

// Reset the board
void resetBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = ' ';
}

// Push a move onto the stack
void push(Stack* stack, int row, int col) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->row = row;
    newNode->col = col;
    newNode->next = stack->top;
    stack->top = newNode;
}

// Pop the last move from the stack
StackNode* pop(Stack* stack) {
    if (stack->top == NULL) return NULL; 
    StackNode* temp = stack->top;
    stack->top = stack->top->next;
    return temp;
}

// Enqueue the current board state into the queue
void enqueue(Queue* queue, char board[SIZE][SIZE]) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) 
            newNode->board[i][j] = board[i][j];
    newNode->next = NULL; 
    if (queue->rear) {
        queue->rear->next = newNode; 
    } else {
        queue->front = newNode; 
    }
    queue->rear = newNode; 
}
