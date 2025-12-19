#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RANGES 1000

typedef struct s_range
{
	long long	start;
	long long	end;
}	t_range;

int	parse_range(char *line, t_range *range)
{
	char	*dash;

	dash = strchr(line, '-');
	if (!dash)
		return (0);
	*dash = '\0';
	range->start = atoll(line);
	range->end = atoll(dash + 1);
	return (1);
}

int	compare_ranges(const void *a, const void *b)
{
	t_range	*r1;
	t_range	*r2;

	r1 = (t_range *)a;
	r2 = (t_range *)b;
	if (r1->start < r2->start)
		return (-1);
	if (r1->start > r2->start)
		return (1);
	return (0);
}

int	merge_ranges(t_range *ranges, int count)
{
	int	i;
	int	j;

	// Sort ranges by start position
	qsort(ranges, count, sizeof(t_range), compare_ranges);
	
	// Merge overlapping ranges
	j = 0;
	i = 1;
	while (i < count)
	{
		// Check if current range overlaps or is adjacent to the last merged range
		if (ranges[i].start <= ranges[j].end + 1)
		{
			// Merge: extend the end if needed
			if (ranges[i].end > ranges[j].end)
				ranges[j].end = ranges[i].end;
		}
		else
		{
			// No overlap, move to next slot
			j++;
			ranges[j] = ranges[i];
		}
		i++;
	}
	return (j + 1);
}

long long	count_fresh_ids(t_range *ranges, int count)
{
	long long	total;
	int			i;

	total = 0;
	i = 0;
	while (i < count)
	{
		// Add the count of IDs in this range (inclusive)
		total += (ranges[i].end - ranges[i].start + 1);
		i++;
	}
	return (total);
}

int	main(void)
{
	FILE		*file;
	char		line[256];
	t_range		ranges[MAX_RANGES];
	int			range_count;
	int			merged_count;
	long long	total_fresh;

	file = fopen("input.txt", "r");
	if (!file)
	{
		printf("Error: Cannot open input.txt\n");
		return (1);
	}
	
	// Read all ranges (stop at empty line)
	range_count = 0;
	while (fgets(line, sizeof(line), file))
	{
		// Remove newline
		line[strcspn(line, "\n")] = '\0';
		
		// Empty line marks end of ranges
		if (strlen(line) == 0)
			break;
		
		if (parse_range(line, &ranges[range_count]))
			range_count++;
	}
	fclose(file);
	
	printf("Original ranges: %d\n", range_count);
	
	// Merge overlapping ranges
	merged_count = merge_ranges(ranges, range_count);
	printf("Merged ranges: %d\n", merged_count);
	
	// Count total fresh IDs
	total_fresh = count_fresh_ids(ranges, merged_count);
	printf("Total fresh ingredient IDs: %lld\n", total_fresh);
	
	return (0);
}