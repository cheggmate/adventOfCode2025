#include <stdio.h>
#include <string.h>

#define MAX_SIZE 150

int main() {
    char grid[MAX_SIZE][MAX_SIZE];
    int rows = 0, cols = 0;
    
    // Read the grid from input
    while (fgets(grid[rows], MAX_SIZE, stdin)) {
        // Remove newline if present
        int len = strlen(grid[rows]);
        if (len > 0 && grid[rows][len-1] == '\n') {
            grid[rows][len-1] = '\0';
            len--;
        }
        if (len > cols) cols = len;
        rows++;
    }
    
    // Count accessible rolls (those with < 4 adjacent rolls)
    int accessible = 0;
    
    // Direction vectors for 8 adjacent positions
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols && grid[i][j] != '\0'; j++) {
            if (grid[i][j] == '@') {
                // Count adjacent rolls
                int adjacent_count = 0;
                
                for (int d = 0; d < 8; d++) {
                    int ni = i + dx[d];
                    int nj = j + dy[d];
                    
                    // Check bounds
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && 
                        grid[ni][nj] == '@') {
                        adjacent_count++;
                    }
                }
                
                // If fewer than 4 adjacent rolls, it's accessible
                if (adjacent_count < 4) {
                    accessible++;
                }
            }
        }
    }
    
    printf("%d\n", accessible);
    
    return 0;
}