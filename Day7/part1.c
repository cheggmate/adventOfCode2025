#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 200
#define MAX_BEAMS 10000

typedef struct s_beam
{
	int	row;
	int	col;
}	t_beam;

typedef struct s_manifold
{
	char	grid[MAX_SIZE][MAX_SIZE];
	int		rows;
	int		cols;
	int		start_col;
}	t_manifold;

typedef struct s_queue
{
	t_beam	beams[MAX_BEAMS];
	int		front;
	int		rear;
	int		size;
}	t_queue;

void	init_queue(t_queue *q)
{
	q->front = 0;
	q->rear = 0;
	q->size = 0;
}

void	enqueue(t_queue *q, int row, int col)
{
	if (q->size >= MAX_BEAMS)
		return;
	q->beams[q->rear].row = row;
	q->beams[q->rear].col = col;
	q->rear = (q->rear + 1) % MAX_BEAMS;
	q->size++;
}

int	dequeue(t_queue *q, t_beam *beam)
{
	if (q->size == 0)
		return (0);
	*beam = q->beams[q->front];
	q->front = (q->front + 1) % MAX_BEAMS;
	q->size--;
	return (1);
}

int	read_manifold(const char *filename, t_manifold *m)
{
	FILE	*file;
	// int		row;
	int		col;
	int		len;

	file = fopen(filename, "r");
	if (!file)
		return (0);
	m->rows = 0;
	m->cols = 0;
	m->start_col = -1;
	while (fgets(m->grid[m->rows], MAX_SIZE, file))
	{
		len = strlen(m->grid[m->rows]);
		if (len > 0 && m->grid[m->rows][len - 1] == '\n')
			m->grid[m->rows][len - 1] = '\0';
		len = strlen(m->grid[m->rows]);
		if (len > m->cols)
			m->cols = len;
		// Find starting position 'S'
		for (col = 0; col < len; col++)
		{
			if (m->grid[m->rows][col] == 'S')
				m->start_col = col;
		}
		m->rows++;
	}
	fclose(file);
	return (1);
}

int	simulate_beam(t_manifold *m)
{
	t_queue	queue;
	t_beam	current;
	int		split_count;
	char	visited[MAX_SIZE][MAX_SIZE];
	int		row;
	int		col;

	// Initialize visited array
	for (row = 0; row < m->rows; row++)
		for (col = 0; col < m->cols; col++)
			visited[row][col] = 0;
	
	split_count = 0;
	init_queue(&queue);
	
	// Start beam from S position, moving down
	enqueue(&queue, 0, m->start_col);
	
	while (dequeue(&queue, &current))
	{
		row = current.row;
		col = current.col;
		
		// Check bounds
		if (row < 0 || row >= m->rows || col < 0 || col >= m->cols)
			continue;
		
		// Check if we've already processed this position
		if (visited[row][col])
			continue;
		
		visited[row][col] = 1;
		
		// Check what's at current position
		if (m->grid[row][col] == '^')
		{
			// Hit a splitter!
			split_count++;
			
			// Create two new beams: left and right
			// Continue from row+1 (next row down) at col-1 and col+1
			if (col - 1 >= 0)
				enqueue(&queue, row + 1, col - 1);
			if (col + 1 < m->cols)
				enqueue(&queue, row + 1, col + 1);
		}
		else
		{
			// Empty space (. or S), beam continues downward
			enqueue(&queue, row + 1, col);
		}
	}
	
	return (split_count);
}

int	main(void)
{
	t_manifold	m;
	int			splits;

	if (!read_manifold("input.txt", &m))
	{
		printf("Error: Cannot open input.txt\n");
		return (1);
	}
	
	printf("Manifold dimensions: %d rows x %d cols\n", m.rows, m.cols);
	printf("Start position: S at column %d\n", m.start_col);
	
	splits = simulate_beam(&m);
	
	printf("Total beam splits: %d\n", splits);
	
	return (0);
}