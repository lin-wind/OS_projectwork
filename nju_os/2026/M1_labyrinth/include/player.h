#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    INVALID
}Direction;


int player_move(Map *map, int player_index, Direction direction);
int find_first_empty(const Map *map, Position *pos);
Direction parse_direction(const char *dir_str);

#endif