#ifndef MAP_H
#define MAP_H

#define MAX_ROWS 100
#define MAX_COLS 100


typedef struct {
    int x;
    int y;
}Position;


typedef struct {
    char grid[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
    Position player[10];
}Map;


int load_labyrinth(Map *map, const char *filename);
void print_map(Map *map);
int is_connectable(Map *map);

#endif