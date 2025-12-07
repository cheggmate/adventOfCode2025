#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

int move_arrow(int *current_pos, char direction, int to_move, int t_num, int *count)
{
    int wheel_size = t_num + 1; // 100
    int zero_crossings = 0;
    
    if (to_move == 0)
        return 0;
    
    // Calculate how many times we pass through 0
    if (direction == 'L')
    {
        // Moving left from current_pos by to_move steps
        // We cross 0 each time we go from position k to position k-1 where k=0
        // This happens when we've moved (current_pos + 1) steps, then every 100 steps after
        
        // How many steps until we first reach 0?
        int steps_to_zero = (*current_pos == 0) ? wheel_size : *current_pos;
        
        if (to_move >= steps_to_zero)
        {
            // We reach 0 at least once
            zero_crossings = 1;
            int remaining = to_move - steps_to_zero;
            // Then we cross 0 again every wheel_size steps
            zero_crossings += remaining / wheel_size;
        }
        
        // Update position
        *current_pos = (*current_pos - (to_move % wheel_size) + wheel_size) % wheel_size;
    }
    else // direction == 'R'
    {
        // Moving right from current_pos by to_move steps
        // We cross 0 when going from 99 to 0
        
        // How many steps until we first reach 0?
        int steps_to_zero = (*current_pos == 0) ? wheel_size : (wheel_size - *current_pos);
        
        if (to_move >= steps_to_zero)
        {
            // We reach 0 at least once
            zero_crossings = 1;
            int remaining = to_move - steps_to_zero;
            // Then we cross 0 again every wheel_size steps
            zero_crossings += remaining / wheel_size;
        }
        
        // Update position
        *current_pos = (*current_pos + (to_move % wheel_size)) % wheel_size;
    }
    
    *count += zero_crossings;
    return 0;
}

int main(void)
{
    int count;
    int start = 50;
    int t_num = 99;
    int fd;
    char input[2] = "";
    char in_string[8];
    char direction;
    size_t read_count;
    int current_pos;
    int to_move;
    int str_pos;

    count = 0;
    direction = 'N';
    current_pos = start;
    to_move = 0;
    str_pos = 0;
    memset(in_string, 0, sizeof(in_string));

    fd = open("input.txt", O_RDONLY);
	// fd = open("example.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    
    while (1)
    {
        read_count = read(fd, input, BUFFER_SIZE);
        input[1] = '\0';
        
        if (read_count <= 0)
        {
            printf("count = %d\n", count);
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
                direction = in_string[0];
                to_move = atoi(&in_string[1]);
                
                move_arrow(&current_pos, direction, to_move, t_num, &count);
                
                memset(in_string, 0, sizeof(in_string));
                str_pos = 0;
            }
        }
    }
    
    close(fd);
    return (0);
}