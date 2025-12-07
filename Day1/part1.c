#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

int move_arrow(int *current_pos, char direction, int to_move, int t_num)
{
    int wheel_size = t_num + 1; // 100

    if (direction == 'L')
    {
        *current_pos = (*current_pos - (to_move % wheel_size) + wheel_size) % wheel_size;
    }
    else // direction == 'R'
    {
        *current_pos = (*current_pos + (to_move % wheel_size)) % wheel_size;
    }
    return (0);
}

int main(void)
{
    int count;
    int start = 50;
    int t_num = 99;
    int fd;
    char input[2] = "";
    char in_string[8];  // Made larger for safety
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
            printf("count = %d\n", count);
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
            // Process the complete instruction
            if (str_pos > 0)
            {
                direction = in_string[0];
                to_move = atoi(&in_string[1]);
                
                // printf("current pos before moving %d steps in '%c' direction: %d. ", 
                //        to_move, direction, current_pos);
                move_arrow(&current_pos, direction, to_move, t_num);
                // printf("after moving: %d\n", current_pos);
                
                if (current_pos == 0)
                    count++;
                
                // Clear the buffer for next instruction
                memset(in_string, 0, sizeof(in_string));
                str_pos = 0;
            }
        }
    }
    
    close(fd);
    return (0);
}