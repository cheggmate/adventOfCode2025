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

long long	build_number_rtl(t_worksheet *ws, int start_col, int end_col)
{
	long long	number;
	long long	multiplier;
	int			row;
	int			col;
	char		digit;

	number = 0;
	multiplier = 1;
	
	// Read columns from RIGHT to LEFT (end_col down to start_col)
	for (col = end_col; col >= start_col; col--)
	{
		// Read digits from BOTTOM to TOP (most significant at top)
		// We need to build the number by reading top to bottom
		// So we first need to find the digits in this column
		for (row = 0; row < ws->num_lines - 1; row++)  // Skip operator line
		{
			if (col < (int)strlen(ws->lines[row]))
			{
				digit = ws->lines[row][col];
				if (isdigit(digit))
				{
					// Add this digit at the current position
					number += (digit - '0') * multiplier;
					multiplier *= 10;
				}
			}
		}
	}
	
	return (number);
}

long long	solve_problem_rtl(t_worksheet *ws, int start_col, int end_col)
{
	long long	result;
	long long	num;
	char		op;
	int			col;
	int			row;
	int			digit_col;
	char		digit;

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
	
	// Read numbers RIGHT-TO-LEFT, one column at a time
	result = 0;
	digit_col = end_col;
	
	while (digit_col >= start_col)
	{
		// Build number from this column (read digits top to bottom)
		num = 0;
		// long long multiplier = 1;
		
		// Read from bottom to top to get correct digit order
		for (row = 0; row < ws->num_lines - 1; row++)  // Skip operator line
		{
			if (digit_col < (int)strlen(ws->lines[row]))
			{
				digit = ws->lines[row][digit_col];
				if (isdigit(digit))
				{
					num = num * 10 + (digit - '0');
				}
			}
		}
		// printf("number extracted: %lld\n", num);
		// Apply operation
		if (num > 0 || (result == 0 && num == 0))  // Include zeros at start
		{
			if (result == 0 && digit_col == end_col)
				result = num;
			else if (op == '+')
				result += num;
			else if (op == '*')
				result *= num;
		}
		
		digit_col--;
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
	int			problem_num;

	grand_total = 0;
	col = 0;
	in_problem = 0;
	start_col = 0;
	problem_num = 0;
	
	while (col <= ws->max_width)
	{
		if (is_column_empty(ws, col))
		{
			if (in_problem)
			{
				problem_result = solve_problem_rtl(ws, start_col, col - 1);
				grand_total += problem_result;
				problem_num++;
				in_problem = 0;
			}
		}
		else
		{
			if (!in_problem)
			{
				start_col = col;
				in_problem = 1;
			}
		}
		col++;
	}
	
	if (in_problem)
	{
		problem_result = solve_problem_rtl(ws, start_col, ws->max_width);
		grand_total += problem_result;
		problem_num++;
	}
	
	printf("Total problems solved: %d\n", problem_num);
	
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