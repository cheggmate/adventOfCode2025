#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SIZE 150

// Direction vectors for 8 adjacent positions
int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

// Count adjacent rolls for a position
int count_adjacent(char grid[][MAX_SIZE], int rows, int cols, int i, int j) {
    int count = 0;
    for (int d = 0; d < 8; d++) {
        int ni = i + dx[d];
        int nj = j + dy[d];
        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && grid[ni][nj] == '@') {
            count++;
        }
    }
    return count;
}

int main() {
    char grid[MAX_SIZE][MAX_SIZE];
    int rows = 0, cols = 0;
    
    // Read the grid from input
    while (fgets(grid[rows], MAX_SIZE, stdin)) {
        int len = strlen(grid[rows]);
        if (len > 0 && grid[rows][len-1] == '\n') {
            grid[rows][len-1] = '\0';
            len--;
        }
        if (len > cols) cols = len;
        rows++;
    }
    
    int total_removed = 0;
    bool removed_any = true;
    
    // Keep removing rolls until no more can be removed
    while (removed_any) {
        removed_any = false;
        
        // Find all accessible rolls in current state
        bool to_remove[MAX_SIZE][MAX_SIZE] = {false};
        int count_this_round = 0;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols && grid[i][j] != '\0'; j++) {
                if (grid[i][j] == '@') {
                    int adjacent = count_adjacent(grid, rows, cols, i, j);
                    if (adjacent < 4) {
                        to_remove[i][j] = true;
                        count_this_round++;
                        removed_any = true;
                    }
                }
            }
        }
        
        // Remove all marked rolls
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (to_remove[i][j]) {
                    grid[i][j] = '.';
                }
            }
        }
        
        total_removed += count_this_round;
    }
    
    printf("%d\n", total_removed);
    
    return 0;
}