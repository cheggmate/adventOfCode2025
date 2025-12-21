#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LIGHTS 20
#define MAX_BUTTONS 50
#define MAX_LINE 1000

typedef struct s_machine
{
	int	num_lights;
	int	target[MAX_LIGHTS];
	int	num_buttons;
	int	buttons[MAX_BUTTONS][MAX_LIGHTS];
}	t_machine;

void	parse_lights(char *str, t_machine *m)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != ']')
	{
		if (str[i] == '#')
			m->target[m->num_lights++] = 1;
		else if (str[i] == '.')
			m->target[m->num_lights++] = 0;
		i++;
	}
}

void	parse_button(char *str, t_machine *m)
{
	int	light;
	int	btn_idx;

	btn_idx = m->num_buttons;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			light = atoi(str);
			m->buttons[btn_idx][light] = 1;
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
	char	button_str[100];
	int		i;

	memset(m, 0, sizeof(t_machine));
	p = strchr(line, '[');
	if (p)
		parse_lights(p + 1, m);
	p = line;
	while ((p = strchr(p, '(')) != NULL)
	{
		p++;
		i = 0;
		while (*p && *p != ')')
			button_str[i++] = *p++;
		button_str[i] = '\0';
		parse_button(button_str, m);
	}
}

typedef uint64_t bitset;

int solve_machine(t_machine *m) {
    int n = m->num_lights;
    int b = m->num_buttons;
    bitset matrix[MAX_LIGHTS];
    
    // Initialize matrix: each row is a light, each bit in the row is a button
    // The (b)-th bit will store the target state (augmented column)
    for (int i = 0; i < n; i++) {
        matrix[i] = 0;
        for (int j = 0; j < b; j++) {
            if (m->buttons[j][i]) matrix[i] |= ((bitset)1 << j);
        }
        if (m->target[i]) matrix[i] |= ((bitset)1 << b);
    }

    int pivot_row = 0;
    int pivot_col[MAX_LIGHTS]; 
    for (int i=0; i<MAX_LIGHTS; i++) pivot_col[i] = -1;

    // Gaussian Elimination
    for (int j = 0; j < b && pivot_row < n; j++) {
        int sel = -1;
        for (int i = pivot_row; i < n; i++) {
            if (matrix[i] & ((bitset)1 << j)) { sel = i; break; }
        }
        if (sel == -1) continue;

        // Swap
        bitset temp = matrix[sel];
        matrix[sel] = matrix[pivot_row];
        matrix[pivot_row] = temp;

        // Eliminate
        for (int i = 0; i < n; i++) {
            if (i != pivot_row && (matrix[i] & ((bitset)1 << j))) {
                matrix[i] ^= matrix[pivot_row];
            }
        }
        pivot_col[pivot_row] = j;
        pivot_row++;
    }

    // Check for consistency
    for (int i = pivot_row; i < n; i++) {
        if (matrix[i] & ((bitset)1 << b)) return INT_MAX; // No solution
    }

    // Find minimum presses (Handling Free Variables)
    int min_presses = INT_MAX;
    int num_pivots = pivot_row;
    int free_vars[MAX_BUTTONS], num_free = 0;
    int is_pivot_col[MAX_BUTTONS] = {0};
    
    for(int i=0; i<num_pivots; i++) is_pivot_col[pivot_col[i]] = 1;
    for(int j=0; j<b; j++) if(!is_pivot_col[j]) free_vars[num_free++] = j;

    // Warning: if num_free is too large, this needs a different approach.
    // However, for this puzzle's constraints, it usually stays low.
    for (uint64_t mask = 0; mask < ((uint64_t)1 << num_free); mask++) {
        int current_presses = 0;
        bitset solution = 0;

        for (int i = 0; i < num_free; i++) {
            if (mask & ((uint64_t)1 << i)) {
                solution |= ((bitset)1 << free_vars[i]);
                current_presses++;
            }
        }

        for (int i = 0; i < num_pivots; i++) {
            int p_col = pivot_col[i];
            int val = (matrix[i] >> b) & 1;
            for (int j = 0; j < num_free; j++) {
                int f_col = free_vars[j];
                if ((matrix[i] >> f_col) & 1) {
                    val ^= (mask >> j) & 1;
                }
            }
            if (val) {
                solution |= ((bitset)1 << p_col);
                current_presses++;
            }
        }
        if (current_presses < min_presses) min_presses = current_presses;
    }

    return min_presses;
}

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
		result = solve_machine(&m);
		
		if (result != INT_MAX)
		{
			total += result;
			count++;
			if (count <= 10)
				printf("Machine %d: %d presses\n", count, result);
		}
		else
		{
			printf("Machine %d: No solution!\n", count + 1);
		}
	}
	
	fclose(file);
	
	printf("\nTotal machines: %d\n", count);
	printf("Total button presses required: %d\n", total);
	
	return (0);
}