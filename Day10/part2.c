#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define MAX_COUNTERS 20
#define MAX_BUTTONS 60
#define MAX_LINE 1000

typedef struct s_machine
{
	int	num_counters;
	int	target[MAX_COUNTERS];
	int	num_buttons;
	int	buttons[MAX_BUTTONS][MAX_COUNTERS];
}	t_machine;

// Back-substitution and Result Calculation
int total_presses = 0;
int solution[MAX_BUTTONS] = {0};
// Global variables for recursion to find minimum in underdetermined systems
int min_total_presses;
int current_solution[MAX_BUTTONS];

// Global state for parsing and solving
int solve_machine_linear(t_machine *m) {
    int n = m->num_counters;
    int b = m->num_buttons;
    double matrix[MAX_COUNTERS][MAX_BUTTONS + 1];

    // Build the augmented matrix [A|b]
    // Rows = counters, Columns = buttons
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < b; j++) {
            matrix[i][j] = (double)m->buttons[j][i];
        }
        matrix[i][b] = (double)m->target[i];
    }

    // Gaussian elimination to Row Echelon Form
    int pivot_row = 0;
    int pivot_col[MAX_BUTTONS];
    memset(pivot_col, -1, sizeof(pivot_col));

    for (int j = 0; j < b && pivot_row < n; j++) {
        int sel = pivot_row;
        for (int i = pivot_row + 1; i < n; i++) {
            if (fabs(matrix[i][j]) > fabs(matrix[sel][j])) sel = i;
        }

        if (fabs(matrix[sel][j]) < 1e-9) continue;

        // Swap rows
        for (int k = 0; k <= b; k++) {
            double tmp = matrix[sel][k];
            matrix[sel][k] = matrix[pivot_row][k];
            matrix[pivot_row][k] = tmp;
        }

        // Eliminate other rows
        for (int i = 0; i < n; i++) {
            if (i != pivot_row) {
                double factor = matrix[i][j] / matrix[pivot_row][j];
                for (int k = j; k <= b; k++) {
                    matrix[i][k] -= factor * matrix[pivot_row][k];
                }
            }
        }
        pivot_col[pivot_row] = j;
        pivot_row++;
    }

 

    for (int i = 0; i < pivot_row; i++) {
        int col = pivot_col[i];
        if (col != -1) {
            double res = matrix[i][b] / matrix[i][col];
            // Rounding to nearest int to handle floating point precision
            solution[col] = (int)(res + 0.5); 
            total_presses += solution[col];
        }
    }

    return total_presses;
}
void	parse_joltages(char *str, t_machine *m)
{
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			m->target[m->num_counters++] = atoi(str);
			while (*str >= '0' && *str <= '9')
				str++;
		}
		else
			str++;
	}
}

void	parse_button(char *str, t_machine *m)
{
	int	counter;
	int	btn_idx;

	btn_idx = m->num_buttons;
	memset(m->buttons[btn_idx], 0, sizeof(m->buttons[btn_idx]));
	
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			counter = atoi(str);
			if (counter < MAX_COUNTERS)
				m->buttons[btn_idx][counter] = 1;
			while (*str >= '0' && *str <= '9')
				str++;
		}
		else
			str++;
	}
	m->num_buttons++;
}

void	parse_machine(char *line, t_machine *m)
{
	char	*p;
	char	*end;
	char	button_str[100];
	int		i;

	memset(m, 0, sizeof(t_machine));
	
	// Find joltages in {curly braces}
	p = strchr(line, '{');
	if (p)
		parse_joltages(p + 1, m);
	
	// Find buttons in (parentheses)
	p = line;
	while ((p = strchr(p, '(')) != NULL)
	{
		p++;
		end = strchr(p, ')');
		if (!end)
			break;
		i = 0;
		while (p < end)
			button_str[i++] = *p++;
		button_str[i] = '\0';
		parse_button(button_str, m);
		p = end + 1;
	}
}


void find_min_presses(t_machine *m, int *button_presses, int button_idx) {
    // If all buttons processed, verify targets
    if (button_idx == m->num_buttons) {
        int total = 0;
        for (int i = 0; i < m->num_counters; i++) {
            int val = 0;
            for (int j = 0; j < m->num_buttons; j++)
                val += m->buttons[j][i] * button_presses[j];
            if (val != m->target[i]) return;
        }
        for (int j = 0; j < m->num_buttons; j++) total += button_presses[j];
        if (total < min_total_presses) min_total_presses = total;
        return;
    }

    // Optimization: This puzzle's math allows solving directly if we have 
    // a square matrix, but since it's small, we can use a bounded search
    // or Gaussian elimination.
}

// Optimized Solver using Gaussian Elimination for Part 2
// int solve_machine_linear(t_machine *m) {
//     double matrix[MAX_COUNTERS][MAX_BUTTONS + 1];
//     int i, j, k;

//     for (i = 0; i < m->num_counters; i++) {
//         for (j = 0; j < m->num_buttons; j++)
//             matrix[i][j] = (double)m->buttons[j][i];
//         matrix[i][m->num_buttons] = (double)m->target[i];
//     }

//     // Standard Gaussian Elimination
//     int pivot_row = 0;
//     for (j = 0; j < m->num_buttons && pivot_row < m->num_counters; j++) {
//         int best = pivot_row;
//         for (i = pivot_row + 1; i < m->num_counters; i++)
//             if (fabs(matrix[i][j]) > fabs(matrix[best][j])) best = i;

//         if (fabs(matrix[best][j]) < 1e-9) continue;

//         for (k = 0; k <= m->num_buttons; k++) {
//             double tmp = matrix[pivot_row][k];
//             matrix[pivot_row][k] = matrix[best][k];
//             matrix[best][k] = tmp;
//         }

//         for (i = 0; i < m->num_counters; i++) {
//             if (i != pivot_row) {
//                 double factor = matrix[i][j] / matrix[pivot_row][j];
//                 for (k = j; k <= m->num_buttons; k++)
//                     matrix[i][k] -= factor * matrix[pivot_row][k];
//             }
//         }
//         pivot_row++;
//     }

//     int total_presses = 0;
//     for (i = 0; i < pivot_row; i++) {
//         // Find the variable index for this row
//         int var_idx = -1;
//         for(int c=0; c < m->num_buttons; c++) {
//             if (fabs(matrix[i][c] - 1.0) < 1e-9) { var_idx = c; break; }
//         }
//         if (var_idx != -1) {
//             double res = matrix[i][m->num_buttons] / matrix[i][var_idx];
//             total_presses += (int)(res + 0.5); // Round to nearest integer press
//         }
//     }
//     return total_presses;
// }

int	main(void)
{
	FILE		*file;
	char		line[MAX_LINE];
	t_machine	m;
	int			total;
	int			result;
	int			count;

	file = fopen("input.txt", "r");
	if (!file)
	{
		printf("Error: Cannot open input.txt\n");
		return (1);
	}
	
	total = 0;
	count = 0;
	
	while (fgets(line, sizeof(line), file))
	{
		if (strlen(line) < 5)
			continue;
		
		parse_machine(line, &m);
		result = solve_machine_linear(&m);
		
		if (result >= 0)
		{
			total += result;
			count++;
			if (count <= 10)
				printf("Machine %d: %d presses (counters: %d)\n", 
					count, result, m.num_counters);
		}
		else
		{
			printf("Machine %d: Failed to solve\n", count + 1);
		}
	}
	
	fclose(file);
	
	printf("\nTotal machines: %d\n", count);
	printf("Total button presses required: %d\n", total);
	
	return (0);
}