#include <stdio.h>
#include <string.h>

#include "player.h"
#include "map.h"



int player_move(Map *map, int player_index, Direction direction) {
    if(map->player[player_index].x == -1) {
        Position pos;
        if(find_first_empty(map, &pos)) {
            map->player[player_index] = pos;
            map->grid[pos.x][pos.y] = '0' + player_index;
            return 0;
        } else {
            fprintf(stderr, "Error: NO empty space to place player %d\n", player_index);
            return 1;
        }
    }

    int cur_x = map->player[player_index].x;
    int cur_y = map->player[player_index].y;
    int new_x = cur_x;
    int new_y = cur_y;

    switch(direction) {
        case UP:
            new_x = cur_x - 1;
            break;
        case DOWN:
            new_x = cur_x + 1;
            break;
        case LEFT:
            new_y = cur_y - 1;
            break;
        case RIGHT:
            new_y = cur_y + 1;
            break;
        default:
            fprintf(stderr, "Error: Invalid direction\n");
            return 1;
    }

    if(new_x < 0 || new_x >= map->rows || new_y < 0 || new_y >= map->cols) {
        fprintf(stderr, "Error: Move out of bounds\n");
        return 1;
    }

    if(map->grid[new_x][new_y] != '.') {
        fprintf(stderr, "Error: Move blocked by wall or another player\n");
        return 1;
    }

    map->grid[cur_x][cur_y] = '.';
    map->grid[new_x][new_y] = '0' + player_index;
    map->player[player_index].x = new_x;
    map->player[player_index].y = new_y;

    return 0;
}

int find_first_empty(const Map *map, Position *pos) {
    for(int x = 0; x < map->rows; x++) {
        for(int y = 0; y < map->cols; y++) {
            if(map->grid[x][y] == '.') {
                pos->x = x;
                pos->y = y;
                return 1;
            }
        }
    }
    return 0;
}

Direction parse_direction(const char *dir_str) {
    if(strcmp(dir_str, "up") == 0) {
        return UP;
    } else if(strcmp(dir_str, "down") == 0) {
        return DOWN;
    } else if(strcmp(dir_str, "left") == 0) {
        return LEFT;
    } else if(strcmp(dir_str, "right") == 0) {
        return RIGHT;
    } else {
        return INVALID;
    }
}