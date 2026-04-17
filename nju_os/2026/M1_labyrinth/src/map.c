#include <stdio.h>

#include "map.h"

int load_labyrinth(Map *map, const char *filename){

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return 1;
    }   

    map->rows = 0;
    map->cols = 0;

    for(int i = 0; i < 10; i++) {
        map->player[i].x = -1;
        map->player[i].y = -1;
    }

    char buffer[MAX_COLS + 2]; 
    while(fgets(buffer, sizeof(buffer), fp) != NULL) {
        int col = 0;
        while(buffer[col] != '\n' && buffer[col] != '\0') {
            if(col >= MAX_COLS) {
                fprintf(stderr, "Error: Too many columns in the map\n");
                fclose(fp);
                return 1;
            }
            map->grid[map->rows][col] = buffer[col];
            if(buffer[col] >= '0' && buffer[col] <= '9') {
                int player_index = buffer[col] - '0';
                map->player[player_index].x = map->rows;
                map->player[player_index].y = col;
            }
            col++;
        }
        map->cols = col;
        map->rows++;
        if(map->rows >= MAX_ROWS) {
            fprintf(stderr, "Error: Too many rows in the map\n");
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}


void print_map(Map *map) {
    for(int x = 0; x < map->rows; x++) {
        for(int y = 0; y < map->cols; y++) {
            printf("%c", map->grid[x][y]);
        }
        printf("\n");
    }
}

int is_connectable(Map *map) {
    if(map -> rows == 0 || map -> cols ==0) {
        return 1;
    }
    int visited[MAX_ROWS][MAX_COLS] = {0};
    Position queue[MAX_ROWS * MAX_COLS];
    int front = 0, rear = 0;

    int start_x = -1, start_y = -1;
    for(int x = 0; x < map->rows; x++) {
        for(int y = 0; y < map->cols; y++) {
            if(map->grid[x][y] != '#') {
                start_x = x;
                start_y = y;
                break;
            }
        }
        if(start_x != -1) {
            break;
        }
    }

    if(start_x == -1) {
        fprintf(stderr, "Error: No starting point found in the map\n");
        return 1;
    }

    queue[rear].x = start_x;
    queue[rear].y = start_y;
    rear++;
    visited[start_x][start_y] = 1;

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while(front < rear) {
        Position current = queue[front];
        front++;

        for(int i = 0; i < 4; i++) {
            int new_x = current.x + directions[i][0];
            int new_y = current.y + directions[i][1];

            if(new_x >= 0 && new_x < map->rows && new_y >= 0 && new_y < map->cols) {
                if(map->grid[new_x][new_y] != '#' && !visited[new_x][new_y]) {
                    visited[new_x][new_y] = 1;
                    queue[rear].x = new_x;
                    queue[rear].y = new_y;
                    rear++;
                }
            }
        }
    }

    for(int x = 0; x < map->rows; x++) {
        for(int y = 0; y < map->cols; y++) {
            if(map->grid[x][y] != '#' && !visited[x][y]) {
                return 1;
            }
        }
    }

    return 0;   
}
