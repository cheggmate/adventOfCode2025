#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RANGES 1000
#define MAX_IDS 1000

typedef struct s_range
{
	long long	start;
	long long	end;
}	t_range;

int	is_fresh(long long id, t_range *ranges, int range_count)
{
	int	i;

	i = 0;
	while (i < range_count)
	{
		if (id >= ranges[i].start && id <= ranges[i].end)
			return (1);
		i++;
	}
	return (0);
}

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

int	main(void)
{
	FILE		*file;
	char		line[256];
	t_range		ranges[MAX_RANGES];
	long long	ids[MAX_IDS];
	int			range_count;
	int			id_count;
	int			fresh_count;
	int			reading_ranges;
	int			i;

	file = fopen("input.txt", "r");
	if (!file)
	{
		printf("Error: Cannot open input.txt\n");
		return (1);
	}
	range_count = 0;
	id_count = 0;
	reading_ranges = 1;
	while (fgets(line, sizeof(line), file))
	{
		// Remove newline
		line[strcspn(line, "\n")] = '\0';
		
		// Empty line marks transition from ranges to IDs
		if (strlen(line) == 0)
		{
			reading_ranges = 0;
			continue;
		}
		if (reading_ranges)
		{
			if (parse_range(line, &ranges[range_count]))
				range_count++;
		}
		else
		{
			ids[id_count] = atoll(line);
			id_count++;
		}
	}
	fclose(file);
	
	// Count fresh ingredients
	fresh_count = 0;
	i = 0;
	while (i < id_count)
	{
		if (is_fresh(ids[i], ranges, range_count))
			fresh_count++;
		i++;
	}
	printf("Total ingredient IDs: %d\n", id_count);
	printf("Fresh ingredient IDs: %d\n", fresh_count);
	return (0);
}