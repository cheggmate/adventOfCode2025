#include <stdint.h>
#include <stdio.h>


typedef struct {uint32_t x; uint32_t y;} vec2;


vec2 tile[512] = {0};
uint16_t tile_len = 0;


uint64_t area(vec2 v0, vec2 v1);

int
main(void)
{
    char line[128];
    while (tile_len<512 && NULL != fgets(line, 128, stdin)) {
        vec2 v;
        sscanf(line, "%d,%d", &v.x, &v.y);
        tile[tile_len++] = v;
    }

    uint64_t A = 0;
    for (uint16_t i = 0; i<tile_len; i++)
        for (uint16_t j = i; j<tile_len; j++) {
            if (i == j) continue;
            A = A > area(tile[i], tile[j]) ? A : area(tile[i], tile[j]);
        }
    
    printf("%llu\n", A);
    return 0;
}


uint64_t
area(vec2 v0, vec2 v1)
{
    uint64_t h = 1 + (v0.y > v1.y  ? v0.y - v1.y : v1.y - v0.y);
    uint64_t w = 1 + (v0.x >= v1.x ? v0.x - v1.x : v1.x - v0.x);
    return h*w;
}