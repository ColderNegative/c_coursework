#include <stdio.h>
#include "graphics.h"

#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 400
#define SCREEN_MARGIN 40

#define MAZE_SIZE 11

#define PATH 0
#define WALL 1
#define START 2
#define END 3

typedef struct {
    int x;
    int y;
} vec2;

void drawBoard(int* board, int size);

int main(void) {
    // test maze; MAZE_SIZE must be 11
    int test_maze[11][11] = {
        {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
        {START, PATH, PATH, PATH, PATH, PATH, WALL, PATH, PATH, PATH, WALL},
        {WALL, PATH, WALL, WALL, WALL, WALL, WALL, PATH, WALL, PATH, WALL},
        {WALL, PATH, PATH, PATH, PATH, PATH, WALL, PATH, WALL, PATH, WALL},
        {WALL, PATH, WALL, WALL, WALL, PATH, WALL, PATH, WALL, PATH, WALL},
        {WALL, PATH, WALL, PATH, WALL, PATH, PATH, PATH, WALL, PATH, WALL},
        {WALL, PATH, WALL, PATH, WALL, WALL, WALL, WALL, WALL, PATH, WALL},
        {WALL, PATH, WALL, PATH, PATH, PATH, WALL, PATH, WALL, PATH, WALL},
        {WALL, PATH, WALL, PATH, WALL, PATH, WALL, PATH, WALL, PATH, WALL},
        {WALL, PATH, WALL, PATH, WALL, PATH, WALL, PATH, PATH, PATH, END},
        {WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL, WALL},
    };

    // set screen
    setWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    drawBoard((int *) test_maze, MAZE_SIZE);

    
}

void drawBoard(int* board, int size) {
    //  choose smaller side; make sure board sits in screen size
    int screen_size;
    if (SCREEN_HEIGHT < SCREEN_WIDTH) {
        screen_size = SCREEN_HEIGHT;
    } else {
        screen_size = SCREEN_WIDTH;
    }

    // spacing
    int spacing = (screen_size - SCREEN_MARGIN*2)/size;

    vec2 current_pos = {SCREEN_MARGIN, SCREEN_MARGIN};
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int type = *((board+i*size) + j);
            switch (type) {
                case WALL:
                    setColour(black);
                    fillRect(current_pos.x, current_pos.y, spacing, spacing);
                    break;
                case START:
                    setColour(blue);
                    fillRect(current_pos.x, current_pos.y, spacing, spacing);
                    break;
                case PATH:
                    setColour(black);
                    drawRect(current_pos.x, current_pos.y, spacing, spacing);
                    break;
                case END:
                    setColour(gray);
                    fillRect(current_pos.x, current_pos.y, spacing, spacing);
                    break;
            }
            current_pos.x += spacing;
        }
        current_pos.y += spacing;
        current_pos.x = SCREEN_MARGIN;
    }


}
