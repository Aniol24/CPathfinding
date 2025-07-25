#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 20

#define GRID_COLS (SCREEN_WIDTH / CELL_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / CELL_SIZE)

typedef struct {
    int row, col;
    int g, h, f;
    int parentRow, parentCol;
    int visited;
} Node;


int numGreenCells = 0;

void findPath(Vector2 start, Vector2 end, int grid[GRID_ROWS][GRID_COLS]) {
    if (numGreenCells < 2) return;

    Node nodes[GRID_ROWS][GRID_COLS] = { 0 };

    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            nodes[row][col].row = row;
            nodes[row][col].col = col;
            nodes[row][col].g = 999999;
            nodes[row][col].visited = 0;
            nodes[row][col].parentRow = -1;
            nodes[row][col].parentCol = -1;
        }
    }

    int startRow = (int)start.y;
    int startCol = (int)start.x;
    int endRow = (int)end.y;
    int endCol = (int)end.x;

    nodes[startRow][startCol].g = 0;
    nodes[startRow][startCol].h = 10 * (abs(endRow - startRow) + abs(endCol - startCol));
    nodes[startRow][startCol].f = nodes[startRow][startCol].h;

    Vector2 openList[GRID_ROWS * GRID_COLS];
    int openCount = 0;
    openList[openCount++] = (Vector2){ startCol, startRow };

    while (openCount > 0) {
        int bestIndex = 0;
        int bestF = 999999;
        for (int i = 0; i < openCount; i++) {
            int r = (int)openList[i].y;
            int c = (int)openList[i].x;
            if (nodes[r][c].f < bestF) {
                bestF = nodes[r][c].f;
                bestIndex = i;
            }
        }

        Vector2 current = openList[bestIndex];
        for (int i = bestIndex; i < openCount - 1; i++) {
            openList[i] = openList[i + 1];
        }
        openCount--;

        int curRow = (int)current.y;
        int curCol = (int)current.x;

        if (curRow == endRow && curCol == endCol) {
            int r = endRow;
            int c = endCol;
            while (!(r == startRow && c == startCol)) {
                DrawRectangle(c * CELL_SIZE, r * CELL_SIZE, CELL_SIZE, CELL_SIZE, GREEN);
                int pr = nodes[r][c].parentRow;
                int pc = nodes[r][c].parentCol;
                r = pr;
                c = pc;
            }
            return;
        }

        nodes[curRow][curCol].visited = 1;

        int directions[8][2] = {
            {0, 1}, {1, 0}, {0, -1}, {-1, 0},
            {1, 1}, {1, -1}, {-1, -1}, {-1, 1}
        };

        for (int i = 0; i < 8; i++) {
            int dCol = directions[i][0];
            int dRow = directions[i][1];
            int nr = curRow + dRow;
            int nc = curCol + dCol;

            if (nr < 0 || nr >= GRID_ROWS || nc < 0 || nc >= GRID_COLS) continue;
            if (grid[nr][nc] == 2 && !(nr == endRow && nc == endCol)) continue;

            int diagonal = (dRow != 0 && dCol != 0);

            if (diagonal) {
                int adj1 = grid[curRow][nc] == 2;
                int adj2 = grid[nr][curCol] == 2;
                if (adj1 || adj2) continue;
            }

            int gNew = nodes[curRow][curCol].g + (diagonal ? 14 : 10);
            int hNew = 10 * (abs(nr - endRow) + abs(nc - endCol));
            int fNew = gNew + hNew;

            if (!nodes[nr][nc].visited && gNew < nodes[nr][nc].g) {
                nodes[nr][nc].g = gNew;
                nodes[nr][nc].h = hNew;
                nodes[nr][nc].f = fNew;
                nodes[nr][nc].parentRow = curRow;
                nodes[nr][nc].parentCol = curCol;

                openList[openCount++] = (Vector2){ nc, nr };
            }
        }
    }

    DrawText("No path found", 10, 10, 20, RED);
}


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pathfinding visualizer");
    SetTargetFPS(60);

    int grid[GRID_ROWS][GRID_COLS] = { 0 };
    Vector2 path[2] = { { -1, -1 }, { -1, -1 } };

    const int buttonRightMargin = 20;
    Rectangle clearRedBtn = { SCREEN_WIDTH - 140 - buttonRightMargin, 10, 130, 30 };

    while (!WindowShouldClose()) {

        bool clickedButton = false;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, clearRedBtn)) {
                for (int row = 0; row < GRID_ROWS; row++) {
                    for (int col = 0; col < GRID_COLS; col++) {
                        if (grid[row][col] == 2) grid[row][col] = 0;
                    }
                }
                clickedButton = true;
            }
        }

        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) && !clickedButton) {
            Vector2 mouse = GetMousePosition();
            int col = mouse.x / CELL_SIZE;
            int row = mouse.y / CELL_SIZE;

            if (col >= 0 && col < GRID_COLS && row >= 0 && row < GRID_ROWS) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (grid[row][col] == 1) {
                        grid[row][col] = 0; 
                    } else if (grid[row][col] == 2) {
                        grid[row][col] = 1;
                    } else if (numGreenCells < 2) {
                        grid[row][col] = 1; 
                    }
                } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    if (grid[row][col] == 2) {
                        grid[row][col] = 0;
                    } else {
                        grid[row][col] = 2;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(DARKGRAY);
        numGreenCells = 0;

        for (int row = 0; row < GRID_ROWS; row++) {
            for (int col = 0; col < GRID_COLS; col++) {
                int x = col * CELL_SIZE;
                int y = row * CELL_SIZE;

                if (grid[row][col] == 1) {
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, GREEN);
                    if (numGreenCells < 2) {
                        path[numGreenCells].x = col;
                        path[numGreenCells].y = row;
                        numGreenCells++;
                    }
                } else if (grid[row][col] == 2) {
                    DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, RED);
                }

                DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, GRAY);
            }
        }

        for (int i = numGreenCells; i < 2; i++) {
            path[i].x = -1;
            path[i].y = -1;
        }

        DrawRectangleRec(clearRedBtn, LIGHTGRAY);
        DrawRectangleLinesEx(clearRedBtn, 2, BLACK);

        const char* buttonText = "Clear Red Cells";
        int textWidth = MeasureText(buttonText, 14);
        int textX = clearRedBtn.x + 10;
        int maxTextX = clearRedBtn.x + clearRedBtn.width - buttonRightMargin - textWidth;
        if (textX > maxTextX) textX = maxTextX;
        DrawText(buttonText, textX, clearRedBtn.y + 7, 16, BLACK);

        findPath(path[0], path[1], grid);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

