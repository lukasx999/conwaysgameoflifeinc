#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#include "raylib.h"


#define RECT_COLOR BLUE
#define RECT_SIZE 5
#define SCREEN_WIDTH  1820
#define SCREEN_HEIGHT 980

#define CELLS_WIDTH SCREEN_WIDTH/RECT_SIZE
#define CELLS_HEIGHT SCREEN_HEIGHT/RECT_SIZE



typedef bool grid_t[CELLS_WIDTH][CELLS_HEIGHT];


// always return 0, if index is out of bounds
static bool get_cell(grid_t grid, int32_t x, int32_t y) {
    return x < 0
    || y < 0
    || x == CELLS_WIDTH
    || y == CELLS_HEIGHT
    ? 0
    : grid[x][y];
}


static void fill_neighbours(bool *neighbours, grid_t grid, size_t x, size_t y) {
    size_t index = 0;
    neighbours[index++] = get_cell(grid, x+1, y  );
    neighbours[index++] = get_cell(grid, x,   y+1);
    neighbours[index++] = get_cell(grid, x+1, y+1);
    neighbours[index++] = get_cell(grid, x-1, y  );
    neighbours[index++] = get_cell(grid, x,   y-1);
    neighbours[index++] = get_cell(grid, x-1, y-1);
    neighbours[index++] = get_cell(grid, x+1, y-1);
    neighbours[index++] = get_cell(grid, x-1, y+1);
}

void grid_update(grid_t grid) {

    grid_t newgrid = { 0 };
    memcpy(newgrid, grid, CELLS_WIDTH * CELLS_HEIGHT);

    for (size_t x = 0; x < CELLS_WIDTH; ++x) {
        for (size_t y = 0; y < CELLS_HEIGHT; ++y) {

            bool neighbours[8] = { 0 };
            fill_neighbours(neighbours, grid, x, y);

            uint32_t alive_neighbours = 0;

            for (size_t i = 0; i < 8; ++i) {
                if (neighbours[i])
                    alive_neighbours++;
            }

            // Rule 1
            // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            if (grid[x][y] && alive_neighbours < 2) {
                newgrid[x][y] = 0;
            }

            // Rule 2
            // Any live cell with two or three live neighbours lives on to the next generation.
            if (grid[x][y] && (alive_neighbours == 3 || alive_neighbours == 2)) {
                newgrid[x][y] = 1;
            }

            // Rule 3
            // Any live cell with more than three live neighbours dies, as if by overpopulation.
            if (grid[x][y] && alive_neighbours > 3) {
                newgrid[x][y] = 0;
            }

            // Rule 4
            // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
            if (!grid[x][y] && alive_neighbours == 3) {
                newgrid[x][y] = 1;
            }

        }
    }

    memcpy(grid, newgrid, CELLS_WIDTH * CELLS_HEIGHT);

}

void grid_fill_random(grid_t grid) {

    for (size_t x = 0; x < CELLS_WIDTH; ++x) {
        for (size_t y = 0; y < CELLS_HEIGHT; ++y) {
            grid[x][y] = rand() % 2 == 0;
        }
    }

}

void render_grid(grid_t grid) {

    for (size_t x = 0; x < CELLS_WIDTH; ++x) {
        for (size_t y = 0; y < CELLS_HEIGHT; ++y) {

            if (grid[x][y]) {
                DrawRectangle(
                    x * RECT_SIZE + SCREEN_WIDTH  / 2 - (CELLS_WIDTH  * RECT_SIZE) / 2,
                    y * RECT_SIZE + SCREEN_HEIGHT / 2 - (CELLS_HEIGHT * RECT_SIZE) / 2,
                    RECT_SIZE,
                    RECT_SIZE,
                    RECT_COLOR
                );
            }

        }
    }
}

void fill_new(grid_t grid) {
    memset(grid, 0, CELLS_WIDTH * CELLS_HEIGHT);

    int x = CELLS_WIDTH / 2;
    int y = CELLS_HEIGHT / 2;
    grid[x  ][y  ] = 1;
    grid[x  ][y+1] = 1;
    grid[x-1][y+1] = 1;
    grid[x  ][y+2] = 1;
    grid[x+1][y+2] = 1;
}

int main(void) {

    srand(time(NULL));

    grid_t grid = { 0 };
    fill_new(grid);



    SetTargetFPS(60);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    float time = 0;
    float delay = 0.05f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(BLACK);

            if (IsKeyDown(KEY_SPACE)) {
                grid_fill_random(grid);
            }

            if (IsKeyDown(KEY_J)) {
                fill_new(grid);
            }

            render_grid(grid);

            float ft = GetFrameTime();
            time += ft;
            if (time > delay) {
                grid_update(grid);
                time = 0;
            }

        }
        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
