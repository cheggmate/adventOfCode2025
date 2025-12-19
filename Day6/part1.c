#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 10
#define MAX_COLS 10000

typedef struct s_worksheet
{
	char	lines[MAX_LINES][MAX_COLS];
	int		num_lines;
	int		max_width;
}	t_worksheet;

int	read_worksheet(const char *filename, t_worksheet *ws)
{
	FILE	*file;
	int		len;

	file = fopen(filename, "r");
	if (!file)
		return (0);
	ws->num_lines = 0;
	ws->max_width = 0;
	while (fgets(ws->lines[ws->num_lines], MAX_COLS, file))
	{
		len = strlen(ws->lines[ws->num_lines]);
		// Remove newline but keep the length
		if (len > 0 && ws->lines[ws->num_lines][len - 1] == '\n')
			ws->lines[ws->num_lines][len - 1] = '\0';
		len = strlen(ws->lines[ws->num_lines]);
		if (len > ws->max_width)
			ws->max_width = len;
		ws->num_lines++;
	}
	fclose(file);
	return (1);
}

int	is_column_empty(t_worksheet *ws, int col)
{
	int	row;

	for (row = 0; row < ws->num_lines; row++)
	{
		if (col < (int)strlen(ws->lines[row]))
		{
			if (ws->lines[row][col] != ' ')
				return (0);
		}
	}
	return (1);
}

long long	parse_number(char *str)
{
	char		buffer[100];
	int			i;
	int			j;

	i = 0;
	j = 0;
	// Skip leading spaces
	while (str[i] && str[i] == ' ')
		i++;
	// Copy digits
	while (str[i] && isdigit(str[i]))
		buffer[j++] = str[i++];
	buffer[j] = '\0';
	if (j == 0)
		return (0);
	return (atoll(buffer));
}

long long	solve_problem(t_worksheet *ws, int start_col, int end_col)
{
	long long	result;
	long long	num;
	char		op;
	int			row;
	int			col;
	int			i;
	char		num_str[100];

	// Extract operator from last line
	op = '\0';
	for (col = start_col; col <= end_col; col++)
	{
		if (col < (int)strlen(ws->lines[ws->num_lines - 1]))
		{
			if (ws->lines[ws->num_lines - 1][col] == '+' || 
				ws->lines[ws->num_lines - 1][col] == '*')
			{
				op = ws->lines[ws->num_lines - 1][col];
				break;
			}
		}
	}
	if (op == '\0')
		return (0);
	
	// Process numbers from top to bottom (excluding operator line)
	result = 0;
	for (row = 0; row < ws->num_lines - 1; row++)
	{
		// Extract number from this column range
		i = 0;
		for (col = start_col; col <= end_col; col++)
		{
			if (col < (int)strlen(ws->lines[row]))
				num_str[i++] = ws->lines[row][col];
			else
				num_str[i++] = ' ';
		}
		num_str[i] = '\0';
		num = parse_number(num_str);
		
		if (num > 0)  // Only process non-zero numbers
		{
			if (result == 0)
				result = num;
			else if (op == '+')
				result += num;
			else if (op == '*')
				result *= num;
		}
	}
	return (result);
}

long long	solve_worksheet(t_worksheet *ws)
{
	long long	grand_total;
	long long	problem_result;
	int			col;
	int			start_col;
	int			in_problem;

	grand_total = 0;
	col = 0;
	in_problem = 0;
	start_col = 0;
	
	while (col <= ws->max_width)
	{
		if (is_column_empty(ws, col))
		{
			// End of a problem
			if (in_problem)
			{
				problem_result = solve_problem(ws, start_col, col - 1);
				grand_total += problem_result;
				in_problem = 0;
			}
		}
		else
		{
			// Start of a new problem
			if (!in_problem)
			{
				start_col = col;
				in_problem = 1;
			}
		}
		col++;
	}
	
	// Handle last problem if worksheet doesn't end with empty column
	if (in_problem)
	{
		problem_result = solve_problem(ws, start_col, ws->max_width);
		grand_total += problem_result;
	}
	
	return (grand_total);
}

int	main(void)
{
	t_worksheet	ws;

	if (!read_worksheet("input.txt", &ws))
	{
		printf("Error: Cannot open input.txt\n");
		return (1);
	}
	
	printf("Worksheet dimensions: %d lines x %d columns\n", 
		ws.num_lines, ws.max_width);
	
	long long grand_total = solve_worksheet(&ws);
	
	printf("Grand total: %lld\n", grand_total);
	
	return (0);
}