#include <stdio.h>
#include <string.h>


#define HEIGHT        256
#define WIDTH         256
#define MAX_VERTICIES 2048


typedef struct {
    long x;
    long y;
} vec2;


ssize_t get_vertex_id_by_pos(vec2 pos);
ssize_t get_num_paths_to_goal(ssize_t id, ssize_t goal);


static char tachyon_manifold[HEIGHT][WIDTH] = { 0 };
static size_t vertex_len = 0;
static vec2 vertex_pos[MAX_VERTICIES] = { 0 };
// Adjacency list represention of DAG.
// [id][0] is lchild, [id][1] is rchild.
static ssize_t vertex_adj[MAX_VERTICIES][2]; // = memset(...)
// for memoization of results in a DFS.
static ssize_t paths_to_goal[MAX_VERTICIES] = {0};


int main(void)
{
    memset(vertex_adj, -1, sizeof (ssize_t) * MAX_VERTICIES * 2);

    for (size_t row = 0;
         row<HEIGHT && NULL != fgets(tachyon_manifold[row], WIDTH, stdin);
         row++) {
        for(size_t col = 0; tachyon_manifold[row][col] != '\0'; col++) {
            char c = tachyon_manifold[row][col];
            if (c == 'S' || c == '^')
                vertex_pos[vertex_len++] = (vec2){.x = (long)col,
                                                  .y = (long)row};
        }
    }

    // Connect sources, sinks, and splitters into a DAG.
    // Handle start to next vertex as special case (beam doesn't split).
    vec2 pos = vertex_pos[0];
    do { pos.y++; } while (tachyon_manifold[pos.y][pos.x] == '.');
    vertex_adj[0][0] = get_vertex_id_by_pos(pos);

    // Create an end vertex as a sink and a search goal.
    vertex_pos[vertex_len++] = (vec2){.x = 0, .y = 0};

    // Handle rest (beams split, each vertex has out-degree 2).
    for (size_t from_id = 1; from_id<vertex_len; from_id++) {
        for(long x_offset = -1; x_offset <= 1; x_offset += 2) {
            vec2 pos = vertex_pos[from_id];
            pos.x += x_offset;
            do { pos.y++; } while (tachyon_manifold[pos.y][pos.x] == '.');
            size_t to_id = get_vertex_id_by_pos(pos);
            // if to_id is -1 the beam has run off the end of tachyon manifold,
            // so connect this vertex to the end vertex.
            vertex_adj[from_id][x_offset < 0 ? 0 : 1] =
                to_id == (size_t)-1 ? vertex_len-1 : to_id;
        }
    }

    printf("%ld\n", get_num_paths_to_goal(0, vertex_len-1));
    return 0;
}


ssize_t
get_vertex_id_by_pos(vec2 pos)
{
    for (size_t i = 0; i<vertex_len; i++)
        if (vertex_pos[i].x == pos.x && vertex_pos[i].y == pos.y)
            return i;
    return -1;
}


// By depth-first search with memoization.
ssize_t
get_num_paths_to_goal(ssize_t id, ssize_t goal)
{
    if (id == goal)
        return 1;

    if (paths_to_goal[id] > 0)
        return paths_to_goal[id];

    ssize_t left = vertex_adj[id][0];
    ssize_t right = vertex_adj[id][1];
    if (left >= 0)
        paths_to_goal[id] += get_num_paths_to_goal(left, goal);

    if (right >= 0)
        paths_to_goal[id] += get_num_paths_to_goal(right, goal);

    return paths_to_goal[id];
}