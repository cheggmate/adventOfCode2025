#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

// Find the largest number by selecting exactly 'count' digits from str
long long find_largest_k_digits(char *str, int count)
{
    int len = strlen(str);
    int selected[200];  // indices of selected digits
    int sel_idx = 0;
    int i, j;
    char max_char;
    int max_pos;
    int start_search;
    
    if (!str || len < count)
        return 0;
    
    start_search = 0;
    
    // Greedily select digits
    while (sel_idx < count)
    {
        // How many more digits do we need?
        int remaining_needed = count - sel_idx;
        
        // We must be able to select remaining_needed digits from position i onwards
        // So we can search up to position: len - remaining_needed
        int search_limit = len - remaining_needed;
        
        // Find the largest digit in the valid range
        max_char = '0' - 1;  // Start with value less than '0'
        max_pos = start_search;
        
        for (i = start_search; i <= search_limit; i++)
        {
            if (str[i] > max_char)
            {
                max_char = str[i];
                max_pos = i;
            }
        }
        
        // Select this digit
        selected[sel_idx++] = max_pos;
        start_search = max_pos + 1;
    }
    
    // Convert selected digits to number
    long long result = 0;
    for (i = 0; i < count; i++)
    {
        result = result * 10 + (str[selected[i]] - '0');
    }
    
    return result;
}

int main(void)
{
    int fd;
    char input[2];
    char in_string[200];
    int str_pos;
    size_t read_count;
    long long total;
    long long bank_value;

    str_pos = 0;
    total = 0;
    memset(in_string, 0, sizeof(in_string));

    fd = open("input.txt", O_RDONLY);
    if (fd < 0)
    {
        printf("Error opening file\n");
        return (1);
    }
    
    while (1)
    {
        read_count = read(fd, input, BUFFER_SIZE);
        input[1] = '\0';
        
        if (read_count <= 0)
        {
            // Process last line if exists
            if (str_pos > 0)
            {
                bank_value = find_largest_k_digits(in_string, 12);
                total += bank_value;
            }
            break;
        }
        
        if (input[0] != '\n')
        {
            in_string[str_pos++] = input[0];
            in_string[str_pos] = '\0';
        }
        else
        {
            if (str_pos > 0)
            {
                bank_value = find_largest_k_digits(in_string, 12);
                total += bank_value;
                str_pos = 0;
                memset(in_string, 0, sizeof(in_string));
            }
        }
    }
    
    close(fd);
    printf("Total: %lld\n", total);
    return (0);
}