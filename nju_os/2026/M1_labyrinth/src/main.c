#include <stdio.h>
#include "cli.h"
#include "map.h"
#include "player.h"

int main(int argc, char *argv[]) {
    Map map;
    GameConfig config;
    Direction direct;

    config.map_name = NULL;
    config.player_num = -1;
    config.direct_select = NULL;
    config.version = 0;

    if(parse_argument(argc, argv, &config) != 0) {
        return 1;
    }

    if(validate_config(&config, argc) != 0) {
        return 1;
    }

    if(config.version == 1) {
        printf("Labyrinth Game 1.0\n");
        return 0;
    }

    if(load_labyrinth(&map, config.map_name) != 0) {
        return 1;
    }



    if(is_connectable(&map) != 0) {
        fprintf(stderr, "Error: The map is not connectable\n");
        return 1;
    }

    if(config.direct_select != NULL) {
        direct = parse_direction(config.direct_select);
        if(player_move(&map, config.player_num, direct) != 0) {
            return 1;
        }
    }

    print_map(&map);
    
    return 0;

}