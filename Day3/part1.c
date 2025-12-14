#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

int	find_largest_number(char *str)
{
	int	largest;
	int	second_largest;
	int	p_largest;
	int	p_second_largest;
	int	i;
	int	curr_number;
	char current_char[2] = "";

	i = 0;
	if (!str)
		return (0);
	current_char[1] = '\0';
	largest = -1;  // Initialize to -1 to handle '0' correctly
	second_largest = -1;
	p_largest = -1;
	p_second_largest = -1;

	// First pass: find the largest digit
	while (str[i])
	{
		current_char[0] = str[i];
		curr_number = atoi(current_char);
		if (curr_number > largest)
		{
			largest = curr_number;
			p_largest = i;
		}
		i++;
	}

	// Second pass: find the largest digit that comes AFTER p_largest
	i = p_largest + 1;
	while (str[i])
	{
		current_char[0] = str[i];
		curr_number = atoi(current_char);
		if (curr_number > second_largest)
		{
			second_largest = curr_number;
			p_second_largest = i;
		}
		i++;
	}

	// If we found a valid second digit after the first, use it
	if (p_second_largest > p_largest)
		return (largest * 10 + second_largest);

	// Otherwise, find the largest digit before p_largest
	i = 0;
	second_largest = -1;
	while (i < p_largest)
	{
		current_char[0] = str[i];
		curr_number = atoi(current_char);
		if (curr_number > second_largest)
		{
			second_largest = curr_number;
			p_second_largest = i;
		}
		i++;
	}

	return (second_largest * 10 + largest);
}

int main(void)
{
    int fd;
    char input[102] = "";
    char in_string[102];  // Made larger for safety
	int str_pos;
    size_t read_count;
	int	total;

	str_pos = 0;
	total = 0;
    memset(in_string, 0, sizeof(in_string));  // Properly initialize

    fd = open("input.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    
    while (1)
    {
        read_count = read(fd, input, BUFFER_SIZE);
        input[1] = '\0';
        
        if (read_count <= 0)
        {
            break;
        }
        
        if (input[0] != '\n')
        {
            // Build up the string character by character
            in_string[str_pos++] = input[0];
            in_string[str_pos] = '\0';
        }
        else
        {
			// printf("string is %s\n", in_string);
			total += find_largest_number(in_string); 
			str_pos = 0;
        }
    }
    
    close(fd);
	printf("total : %d\n", total);
    return (0);
}
