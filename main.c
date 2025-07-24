#include <stdio.h>
#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 20

#define GRID_COLS (SCREEN_WIDTH / CELL_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / CELL_SIZE)

void findPath(Vector2 start, Vector2 end, int grid[GRID_ROWS][GRID_COLS]) {
    
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pathfinding visualizer");
    SetTargetFPS(60);

    
    int grid[GRID_ROWS][GRID_COLS] = { 0 };
    int numGreenCells = 0;
    Vector2 path[2] = { { -1, -1 }, { -1, -1 } };

    while (!WindowShouldClose()) {
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            int col = mouse.x / CELL_SIZE;
            int row = mouse.y / CELL_SIZE;

            if (col >= 0 && col < GRID_COLS && row >= 0 && row < GRID_ROWS && (numGreenCells < 2 || grid[row][col] % 2 == 1)) {
                grid[row][col] += 1; 
            }
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        numGreenCells = 0;

        for (int row = 0; row < GRID_ROWS; row++) {
            for (int col = 0; col < GRID_COLS; col++) {
                int x = col * CELL_SIZE;
                int y = row * CELL_SIZE;

                if (grid[row][col]%2 == 1) {
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, GREEN);
                    path[numGreenCells].x = col;
                    path[numGreenCells].y = row;
                    numGreenCells++;
                }


                DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, GRAY);
            }
        }

        findPath(path[0], path[1], grid);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

