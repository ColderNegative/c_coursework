#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "graphics.h"

#define SCREEN_SIZE 400
#define SCREEN_MARGIN 20

#define MAZE_SIZE 15

// stack init
#define CAPACITY MAZE_SIZE*MAZE_SIZE
int top = -1;
int stack[CAPACITY];

typedef struct {
    int row;
    int column;
    bool visted;
    // top, right, bottom, left
    bool walls[4];
} cell;

void debug();

// grid functions
void drawGrid(cell* grid);
int indexGrid(int i, int j);

// generation algorithm
int checkNeighborsAlgo(cell* grid, cell current);
void removeWalls(int index1, int index2, cell* grid);
void genMaze(cell* grid);

// solving function
int checkNeighborsSolver(cell* grid, cell current);


// stack functions
void push(int x);
int pop();
int peek();
bool isEmpty();
bool isFull();

int main(void) {
    // init
    time_t t;
    srand((unsigned) time(&t));
    setWindowSize(SCREEN_SIZE, SCREEN_SIZE);

    debug();

}

void drawGrid(cell* grid) {
    int cell_width = (SCREEN_SIZE - SCREEN_MARGIN*2) / MAZE_SIZE;
    for (int i = 0; i < MAZE_SIZE*MAZE_SIZE; i++) {
        int x = grid[i].column*cell_width+SCREEN_MARGIN;
        int y = grid[i].row*cell_width+SCREEN_MARGIN;
        if (grid[i].visted == true) {
            setColour(gray);
            fillRect(x, y, cell_width, cell_width);
        }
        if (*(grid[i].walls)){
            setColour(blue);
            drawLine(x, y, x+cell_width, y);
        }
        if (*(grid[i].walls+1)) {
            setColour(blue);
            drawLine(x+cell_width, y, x+cell_width, y+cell_width);
        }
        if (*(grid[i].walls+2)) {
            setColour(blue);
            drawLine(x, y+cell_width, x+cell_width, y+cell_width);
        }
        if (*(grid[i].walls+3)) {
            setColour(blue);
            drawLine(x, y, x, y+cell_width);
        }
    }
}

int indexGrid(int i, int j) {
    if (i < 0 || j < 0 || i > MAZE_SIZE-1 || j > MAZE_SIZE-1) {
        return -1;
    }

    return i + j * MAZE_SIZE;
}

int checkNeighborsAlgo(cell* grid, cell current) {
    // top right bottom left
    bool neighbors[4] = {false};
    int neigbors_count = 0;

    int top = indexGrid(current.column, current.row+1);
    int right = indexGrid(current.column+1, current.row);
    int bottom = indexGrid(current.column, current.row-1);
    int left = indexGrid(current.column-1, current.row);
    
    //printf("%d, %d, %d, %d\n", top, right, bottom, left);
    if (top != -1 && !grid[top].visted) {
        //printf("top\n");
        neigbors_count++;
        neighbors[0] = true;
    }
    if (right != -1 && !grid[right].visted) {
        //printf("right\n");
        neigbors_count++;
        neighbors[1] = true;
    }
    if (bottom != -1 && !grid[bottom].visted) {
        //printf("bottom\n");
        neigbors_count++;
        neighbors[2] = true;
    }
    if (left != -1 && !grid[left].visted) {
        //printf("left\n");
        neigbors_count++;
        neighbors[3] = true;
    }
    
    if (neigbors_count > 0) {
        int select = rand() % neigbors_count + 1;
        int select_true = 0;
        for (int i = 0; i < 4; i++) {
            if (neighbors[i] == true) {
                select_true++;
            } 
            if (select_true == select) {
                switch (i) {
                    case 0:
                        return top;
                        break;
                    case 1:
                        return right;
                        break;
                    case 2:
                        return bottom;
                        break;
                    case 3:
                        return left;
                        break;
                }
            }
        }
    }

    return indexGrid(current.column, current.row);

}

bool cellEqual(cell one, cell two) {
    if (one.row == two.row && one.column == two.column) {
        return true;
    }
    return false;
}

void removeWalls(int index1, int index2, cell* grid) {
    int x = grid[index1].column - grid[index2].column;
    //printf("%d\n", x);
    if (x == 1) {
        *(grid[index1].walls+3) = false;
        *(grid[index2].walls+1) = false;
    } else if (x == -1) {
        *(grid[index1].walls+1) = false;
        *(grid[index2].walls+3) = false;
    }

    int y = grid[index1].row - grid[index2].row;
    if (y == 1) {
        *(grid[index1].walls) = false;
        *(grid[index2].walls+2) = false;
    } else if (y == -1) {
        *(grid[index1].walls+2) = false;
        *(grid[index2].walls) = false;
    }
}

void genMaze(cell* grid) {
    int current = indexGrid(grid[0].column, grid[0].row);
    while (true) {
        drawGrid(grid);

        grid[current].visted = true;
        int next = checkNeighborsAlgo(grid, grid[current]);
        if (!(current == next)) {
            grid[next].visted = true;
            push(current);
            removeWalls(current, next, grid);
            current = next;
        } else if (!isEmpty()){
            current = pop();
        } else {
            break;
        }

        sleep(20);
    }
}

int checkNeighborsSolver(cell* grid, cell current) {
    // top right bottom left
    bool neighbors[4] = {false};
    int neigbors_count = 0;

    int top = indexGrid(current.column, current.row+1);
    int right = indexGrid(current.column+1, current.row);
    int bottom = indexGrid(current.column, current.row-1);
    int left = indexGrid(current.column-1, current.row);
    
    //printf("%d, %d, %d, %d\n", top, right, bottom, left);
    if (top != -1 && current.walls[0] != true) {
        //printf("top\n");
        neigbors_count++;
        neighbors[0] = true;
    }
    if (right != -1 && current.walls[1] != true) {
        //printf("right\n");
        neigbors_count++;
        neighbors[1] = true;
    }
    if (bottom != -1 && current.walls[2] != true) {
        //printf("bottom\n");
        neigbors_count++;
        neighbors[2] = true;
    }
    if (left != -1 && current.walls[3] != true) {
        //printf("left\n");
        neigbors_count++;
        neighbors[3] = true;
    }
    
    if (neigbors_count > 0) {
        int select = rand() % neigbors_count + 1;
        int select_true = 0;
        for (int i = 0; i < 4; i++) {
            if (neighbors[i] == true) {
                select_true++;
            } 
            if (select_true == select) {
                switch (i) {
                    case 0:
                        return top;
                        break;
                    case 1:
                        return right;
                        break;
                    case 2:
                        return bottom;
                        break;
                    case 3:
                        return left;
                        break;
                }
            }
        }
    }

    return indexGrid(current.column, current.row);


}

void push(int x) {
    if (top == CAPACITY-1) {
        return;
    } else {
        top++;
        stack[top] = x;
    }
}

int pop() {
    if (top == -1) {
        return -1;
    } else {
        int x = stack[top];
        top--;
        return x;
    }
}

int peek() {
    return stack[top];
}

bool isEmpty() {
    if (top == -1) {
        return true;
    } else {
        return false;
    }
}

bool isFull() {
    if (top == CAPACITY-1) {
        return true;
    } else {
        return false;
    }
}

void debug() {
    cell grid[MAZE_SIZE*MAZE_SIZE];
    int index = 0;
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            cell new = {
                i,
                j,
                false,
                {true, true, true, true},
            };
            grid[index] = new;
            index++;
        }
    }

    genMaze(grid);

}

