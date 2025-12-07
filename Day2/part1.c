
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
	int 	i;
	int		b_len = strlen(buffer);
	
	i = 0;
	while (i < b_len)
	{
		buffer[i] = '\0';
		i++;
	}
	return (0);
}

int	check_num(long long *sum, char *s_num, int num_len, long long num)
{
	int	half_num_len = num_len / 2;
	int	i;
	
	i = 0;
	while (i < half_num_len)
	{
		if (s_num[i] == s_num[i + half_num_len])
			i++;
		else
			return (1);
	}
	printf("adding %lld (%s)\n", num, s_num);
	(*sum) += num;
	return (0);
}

int	compare_nums(long long *sum, long long start_num, long long end_num)
{

	size_t 	num_len;
	char	s_num[50] = "";
	long long		curr;

	num_len = 0;
	curr = start_num;
	if (start_num > end_num)
		return (1);
	if (start_num == end_num)
	{
		// itoa(start_num, s_num, 10);
		sprintf(s_num, "%lld", start_num);
		num_len = strlen(s_num);
		check_num(sum, s_num, num_len, start_num);
		// if (num_len % 2 != 0)
		// 	return (0);
		// else
		// {
		// 	check_num(sum, s_num, num_len, start_num);
		// 	return (0);
		// }
		// free(s_num);
		return (0);
	}
	else
	{
		while (curr <= end_num)
		{
			sprintf(s_num, "%lld", curr);
			num_len = strlen(s_num);
			if (num_len % 2 == 0)
				check_num(sum, s_num, num_len, curr);
			curr++;
		}
	}
	return (0);
}

int	main(void)
{
	int fd;
	long long	sum;
	char	buffer[25] = "";
	char	input[2] = "";
	long long		start_num;	// first_num in the string
	long long		end_num;	// end_num in the string
	size_t	bytes_read;

	sum = 0;
	fd = open("input.txt", O_RDONLY);
	if (fd <= 0)
		return (1);
	while (1)
	{
		bytes_read = read(fd, input, BUFFER_SIZE);
		if (bytes_read <= 0)
		{
			printf("total: %lld\n", sum);
			break ;
		}
		input[1] = '\0';
		if (input[0] == '-')
		{
			printf("found a dash\n");
			start_num = atoll(buffer);
			printf("start_num = %lld\n", start_num);
			clear_buffer(buffer);
			// printf("buffer: %s\n", buffer);
		}
		else if (input[0] == ',')
		{
			printf("found a comma\n");
			end_num = atoll(buffer);
			printf("end num = %lld\n", end_num);
			compare_nums(&sum, start_num, end_num);
			// printf("done compare nums\n");
			clear_buffer(buffer);
			// printf("buffer: %s\n", buffer);
		}
		else
		{
			// printf("catting buffer\n");
			strlcat(buffer, input, 25);
		}
	}
	
}
