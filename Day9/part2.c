#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POINTS 500

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

int	read_points(const char *filename, t_point *points)
{
	FILE	*file;
	int		count;

	file = fopen(filename, "r");
	if (!file)
		return (0);
	count = 0;
	while (fscanf(file, "%d,%d", &points[count].x, &points[count].y) == 2)
		count++;
	fclose(file);
	return (count);
}

// Check if point is on the boundary (path from points list)
int	is_on_boundary(t_point *points, int n, int x, int y)
{
	int	i;
	int	x1, y1, x2, y2;

	for (i = 0; i < n; i++)
	{
		x1 = points[i].x;
		y1 = points[i].y;
		x2 = points[(i + 1) % n].x;
		y2 = points[(i + 1) % n].y;
		
		// Check if (x,y) is on line segment from (x1,y1) to (x2,y2)
		if (x1 == x2 && x1 == x)  // Vertical line
		{
			if ((y >= y1 && y <= y2) || (y >= y2 && y <= y1))
				return (1);
		}
		else if (y1 == y2 && y1 == y)  // Horizontal line
		{
			if ((x >= x1 && x <= x2) || (x >= x2 && x <= x1))
				return (1);
		}
	}
	return (0);
}

// Ray casting algorithm: count intersections to determine if inside polygon
// int	is_inside_or_on_boundary(t_point *points, int n, int x, int y)
// {
// 	int	i;
// 	int	j;
// 	int	count;
// 	int	x1, y1, x2, y2;

// 	// First check if on boundary
// 	if (is_on_boundary(points, n, x, y))
// 		return (1);
	
// 	// Ray casting: cast ray from point to the right, count intersections
// 	count = 0;
// 	for (i = 0, j = n - 1; i < n; j = i++)
// 	{
// 		x1 = points[i].x;
// 		y1 = points[i].y;
// 		x2 = points[j].x;
// 		y2 = points[j].y;
		
// 		// Check if ray crosses this edge
// 		if (((y1 > y) != (y2 > y)) &&
// 			(x < (x2 - x1) * (y - y1) / (y2 - y1) + x1))
// 			count++;
// 	}
	
// 	// Odd count means inside
// 	return (count % 2 == 1);
// }
int is_green_or_red(t_point *points, int n, int x, int y) {
    int i, j, c = 0;
    for (i = 0, j = n-1; i < n; j = i++) {
        // Check if point is exactly on a boundary segment (Red Tile)
        if (((points[i].y <= y && y < points[j].y) || (points[j].y <= y && y < points[i].y)) &&
            (x < (points[j].x - points[i].x) * (y - points[i].y) / (points[j].y - points[i].y) + points[i].x)) {
            c = !c;
        }
        // Specific check for horizontal/vertical boundary alignment
        if (points[i].x == points[j].x && x == points[i].x) {
            if (y >= (points[i].y < points[j].y ? points[i].y : points[j].y) &&
                y <= (points[i].y > points[j].y ? points[i].y : points[j].y)) return 1;
        }
        if (points[i].y == points[j].y && y == points[i].y) {
            if (x >= (points[i].x < points[j].x ? points[i].x : points[j].x) &&
                x <= (points[i].x > points[j].x ? points[i].x : points[j].x)) return 1;
        }
    }
    return c; // 1 if inside (green), 0 if outside
}

// Check if entire rectangle only contains red/green tiles
// int	is_valid_rectangle(t_point *points, int n, int x1, int y1, int x2, int y2)
// {
// 	int	x;
// 	int	y;
// 	int	min_x, max_x, min_y, max_y;

// 	// Ensure x1 <= x2 and y1 <= y2
// 	min_x = (x1 < x2) ? x1 : x2;
// 	max_x = (x1 < x2) ? x2 : x1;
// 	min_y = (y1 < y2) ? y1 : y2;
// 	max_y = (y1 < y2) ? y2 : y1;
	
// 	// Check all points in rectangle
// 	for (y = min_y; y <= max_y; y++)
// 	{
// 		for (x = min_x; x <= max_x; x++)
// 		{
// 			if (!is_inside_or_on_boundary(points, n, x, y))
// 				return (0);
// 		}
// 	}
// 	return (1);
// }

int is_valid_rectangle(t_point *points, int n, int x1, int y1, int x2, int y2) {
    int min_x = (x1 < x2) ? x1 : x2;
    int max_x = (x1 < x2) ? x2 : x1;
    int min_y = (y1 < y2) ? y1 : y2;
    int max_y = (y1 < y2) ? y2 : y1;

    // 1. Check the four corners of the potential rectangle
    if (!is_green_or_red(points, n, min_x, min_y)) return 0;
    if (!is_green_or_red(points, n, max_x, min_y)) return 0;
    if (!is_green_or_red(points, n, min_x, max_y)) return 0;
    if (!is_green_or_red(points, n, max_x, max_y)) return 0;

    // 2. Check if any boundary vertex is strictly INSIDE the rectangle
    // If a boundary vertex is inside, the rectangle contains "empty" space
    for (int i = 0; i < n; i++) {
        if (points[i].x > min_x && points[i].x < max_x &&
            points[i].y > min_y && points[i].y < max_y) {
            return 0;
        }
        
        // 3. Check if any boundary edge cuts THROUGH the rectangle
        // int next = (i + 1) % n;
        // (Simplified check: since edges are axis-aligned, they can only cut through 
        // if they cross the rectangle's boundary or are contained within it)
        // For this specific puzzle, checking if the midpoint of the rectangle is green 
        // is a strong heuristic when combined with the vertex check.
    }
    
    // Final check: Is the center of the rectangle green?
    if (!is_green_or_red(points, n, (min_x + max_x) / 2, (min_y + max_y) / 2)) return 0;

    return 1;
}

int	find_largest_rectangle(t_point *points, int n)
{
	int	i;
	int	j;
	int	x1, y1, x2, y2;
	int	width, height, area;
	int	max_area;

	max_area = 0;
	
	// Try all pairs of red tiles (vertices in the list)
	for (i = 0; i < n; i++)
	{
		x1 = points[i].x;
		y1 = points[i].y;
		
		for (j = i + 1; j < n; j++)
		{
			x2 = points[j].x;
			y2 = points[j].y;
			
			// Skip if same row or same column (not opposite corners)
			if (x1 == x2 || y1 == y2)
				continue;
			
			// Check if this rectangle is valid
			if (is_valid_rectangle(points, n, x1, y1, x2, y2))
			{
				width = (x1 > x2) ? (x1 - x2) : (x2 - x1);
				height = (y1 > y2) ? (y1 - y2) : (y2 - y1);
				area = (width + 1) * (height + 1);
				
				if (area > max_area)
				{
					max_area = area;
					printf("Found rectangle: (%d,%d) to (%d,%d) with area %d\n",
						x1, y1, x2, y2, area);
				}
			}
		}
	}
	
	return (max_area);
}

int	main(void)
{
	t_point	points[MAX_POINTS];
	int		num_points;
	int		max_area;

	num_points = read_points("input.txt", points);
	if (num_points == 0)
	{
		printf("Error: Cannot read input.txt\n");
		return (1);
	}
	
	printf("Number of red tiles (vertices): %d\n", num_points);
	printf("Searching for largest rectangle...\n\n");
	
	max_area = find_largest_rectangle(points, num_points);
	
	printf("\nLargest rectangle area: %d\n", max_area);
	printf("last answer is incorrect\n");
	
	return (0);
}