#include <math.h>   // powf, sqrtf
#include <stdio.h>
#include <stdint.h> // uint32_t
#include <stdlib.h> // qsort


#define   SETBIT(A,k) (A[(k/32)] |=  (1<<(k%32)))
#define UNSETBIT(A,k) (A[(k/32)] &= ~(1<<(k%32)))
#define  TESTBIT(A,k) (A[(k/32)] &   (1<<(k%32)))


typedef struct {uint32_t x; uint32_t y; uint32_t z;} vec3;
typedef struct {uint16_t id0; uint16_t id1; float d;} threeple; // 1-ple, tuple, 3-ple.


static vec3 box[1000] = {0};
static size_t box_len = 0;
static threeple pair[499500] = {0}; // Combinations(1000,2) = 499,500.
static size_t pair_len = 0;
static uint32_t connected[32] = {0}; // 32 * (32 bit) = 1024 bits = 128 bytes.


static int threeplecmp(const void *a, const void *b);
inline static float dist(vec3 u, vec3 v);


int main(void)
{
    char line[128];
    while (box_len<1000 && NULL != fgets(line, 128, stdin)) {
        vec3 v;
        sscanf(line, "%d,%d,%d", &v.x, &v.y, &v.z);
        box[box_len++] = v;
    }

    // Calculate distances for every possible pair of boxes.
    for (size_t i = 0; i<box_len; i++)
        for (size_t j = i; j<box_len; j++) {
            if (i == j) continue;
            pair[pair_len++] =
                (threeple){(uint16_t)i, (uint16_t)j, dist(box[i], box[j])};
        }

    // Sort by distance.
    qsort(pair, pair_len, sizeof (pair[0]), threeplecmp);

    // For every pair of junction boxes, from closest to most distant...
    for (size_t i = 0; i<pair_len; i++) {
        // ...connect the pair...
        uint16_t id0 = pair[i].id0;
        uint16_t id1 = pair[i].id1;
        SETBIT(connected, id0);
        SETBIT(connected, id1);

        // ...determine if all boxes are connected...
        int all_connected = 1;
        for (size_t j = 0; j < box_len; j++)
            all_connected &= TESTBIT(connected, j) ? 1 : 0;

        // ...and if so, report product of the x coords
        // of the last connected pair.
        if (all_connected) {
            printf("%d\n", box[id0].x * box[id1].x);
            break;
        }
    }

    return 0;
}


inline static
float
dist(vec3 u, vec3 v)
{
    return sqrtf(powf(((float)u.x - (float)v.x), 2)+
                 powf(((float)u.y - (float)v.y), 2)+
                 powf(((float)u.z - (float)v.z), 2));
}


int threeplecmp(const void *a, const void*b)
{
    threeple *A = (threeple *)a;
    threeple *B = (threeple *)b;

    if (A->d < B->d) return -1;
    if (A->d > B->d) return 1;
    return 0;
}