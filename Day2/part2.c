#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

int	clear_buffer(char *buffer)
{
	buffer[0] = '\0';
	return (0);
}

// Check if the string is made of a pattern repeated multiple times
int	is_pattern_repeated(char *s_num, int num_len, int pattern_len)
{
	int	i;
	int	j;
	
	// Check if num_len is divisible by pattern_len
	if (num_len % pattern_len != 0)
		return (0);
	
	// Check if the pattern repeats throughout the entire string
	for (i = 0; i < num_len; i++)
	{
		j = i % pattern_len;  // Position in the pattern
		if (s_num[i] != s_num[j])
			return (0);
	}
	
	return (1);
}

// Check if a number is invalid (repeated pattern at least twice)
int	is_invalid_id(char *s_num, int num_len)
{
	int	pattern_len;
	
	// Try all possible pattern lengths from 1 to num_len/2
	// Pattern must repeat at least twice, so max pattern length is num_len/2
	for (pattern_len = 1; pattern_len <= num_len / 2; pattern_len++)
	{
		if (is_pattern_repeated(s_num, num_len, pattern_len))
			return (1);  // Found a repeating pattern
	}
	
	return (0);  // No repeating pattern found
}

void	check_range(long long *sum, long long start_num, long long end_num)
{
	char		s_num[25];
	int			num_len;
	long long	curr;

	curr = start_num;
	
	if (start_num > end_num)
		return;
	
	while (curr <= end_num)
	{
		sprintf(s_num, "%lld", curr);
		num_len = strlen(s_num);
		
		if (is_invalid_id(s_num, num_len))
		{
			printf("Found invalid ID: %lld\n", curr);
			*sum += curr;
		}
		curr++;
	}
}

int	main(void)
{
	int			fd;
	long long	sum;
	char		buffer[25] = "";
	char		input[2] = "";
	long long	start_num;
	long long	end_num;
	ssize_t		bytes_read;

	sum = 0;
	start_num = 0;
	end_num = 0;
	fd = open("input.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
	
	while (1)
	{
		bytes_read = read(fd, input, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			// Process last number if buffer has content
			if (strlen(buffer) > 0)
			{
				end_num = atoll(buffer);
				printf("\nProcessing last range: %lld-%lld\n", start_num, end_num);
				check_range(&sum, start_num, end_num);
			}
			printf("\n=========================\n");
			printf("Total sum: %lld\n", sum);
			printf("=========================\n");
			break;
		}
		input[1] = '\0';
		
		if (input[0] == '-')
		{
			start_num = atoll(buffer);
			printf("\nProcessing range starting at: %lld", start_num);
			clear_buffer(buffer);
		}
		else if (input[0] == ',')
		{
			end_num = atoll(buffer);
			printf("-%lld\n", end_num);
			check_range(&sum, start_num, end_num);
			clear_buffer(buffer);
		}
		else if (input[0] >= '0' && input[0] <= '9')
		{
			strlcat(buffer, input, 25);
		}
	}
	
	close(fd);
	return (0);
}