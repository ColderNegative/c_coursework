#include <stdio.h>
#include "graphics.h"

#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 400
#define SCREEN_MARGIN 20

#define MAZE_SIZE 11

#define ANIMATION_DELAY_MS 100

typedef enum {
    PATH,
    WALL,
    START,
    END,
} blockType;

typedef enum {
    forward,
    backword,
    up,
    down,
    end,
} direction;

typedef struct {
    int x;
    int y;
} vec2;

typedef struct {
    int* board;
    int block_size;
    vec2 start;
    vec2 end;
} board;

typedef struct {
    direction dir;
    vec2 pos;
} robot;

void debug();
board setupBoard(int* board);
void drawBoard(board board);
void drawRobot(robot pathfinder, board maze);
void animateRobot(int* robo_path, board maze, robot pathfinder);

int main(void) {
    // set screen
    setWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

    debug();

}

board setupBoard(int* board_init) {
    int* temp = board_init;

    //  choose smaller side; make sure board sits in screen size
    int screen_size;
    if (SCREEN_HEIGHT < SCREEN_WIDTH) {
        screen_size = SCREEN_HEIGHT;
    } else {
        screen_size = SCREEN_WIDTH;
    }

    int box_size = (screen_size - SCREEN_MARGIN*2)/MAZE_SIZE;
    vec2 start;
    vec2 end;

    vec2 current_pos = {SCREEN_MARGIN, SCREEN_MARGIN};
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            int type = *((board_init+i*MAZE_SIZE) + j);
            if (type == START) {
                start = current_pos; 
            } else if (type == END) {
                end = current_pos;
            }
            current_pos.x += box_size;
        }
        current_pos.x = SCREEN_MARGIN;
        current_pos.y += box_size;
    }

    board final = {temp, box_size, start, end};
    drawBoard(final);
    return final;
}

void drawBoard(board board) {
    int spacing = board.block_size;

    vec2 current_pos = {SCREEN_MARGIN, SCREEN_MARGIN};
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            int type = *((board.board+i*MAZE_SIZE) + j);
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

void drawRobot(robot pathfinder, board maze) {
    int bounds = maze.block_size;
    vec2 pos = pathfinder.pos;
    int xs[3];
    int ys[3];
    switch (pathfinder.dir) {
        case forward:
            xs[0] = pos.x;
            xs[1] = pos.x+bounds;
            xs[2] = pos.x;
            ys[0] = pos.y;
            ys[1] = pos.y+0.5*bounds;
            ys[2] = pos.y+bounds;
            break;
        case backword:
            xs[0] = pos.x+bounds;
            xs[1] = pos.x;
            xs[2] = pos.x+bounds;
            ys[0] = pos.y;
            ys[1] = pos.y+0.5*bounds;
            ys[2] = pos.y+bounds;
            break;
        case down:
            xs[0] = pos.x;
            xs[1] = pos.x+0.5*bounds;
            xs[2] = pos.x+bounds;
            ys[0] = pos.y;
            ys[1] = pos.y+bounds;
            ys[2] = pos.y;
            break;
        case up:
            xs[0] = pos.x;
            xs[1] = pos.x+0.5*bounds;
            xs[2] = pos.x+bounds;
            ys[0] = pos.y+bounds;
            ys[1] = pos.y;
            ys[2] = pos.y+bounds;
            break;
        case end:
            break;

    }
    setColour(red);
    fillPolygon(3, (int*)xs, (int*)ys);
}

void animateRobot(int* robo_path, board maze, robot pathfinder) {
    int bounds = maze.block_size;
    vec2 start = maze.start;
    while(*robo_path++ != end) {
        switch (*robo_path) {
            case forward:
                pathfinder.pos.x += bounds;
                pathfinder.dir = forward;
                drawRobot(pathfinder, maze);
                break;
            case backword:
                pathfinder.pos.x -= bounds;
                pathfinder.dir = backword;
                drawRobot(pathfinder, maze);
                break;
            case up:
                pathfinder.pos.y -= bounds;
                pathfinder.dir = up;
                drawRobot(pathfinder, maze);
                break;
            case down:
                pathfinder.pos.y += bounds;
                pathfinder.dir = down;
                drawRobot(pathfinder, maze);
                break;
            case end:
                break;
        }
        sleep(ANIMATION_DELAY_MS);
        setColour(white);
        fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        drawBoard(maze);
    }
    drawRobot(pathfinder, maze);
}


void debug() {
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

    int test_path[5] = {forward, forward, forward, backword, end};

    board test = setupBoard((int*)test_maze);
    
    robot pathfinder = {forward, test.start};
    drawRobot(pathfinder, test);

    animateRobot(test_path, test, pathfinder);
}
