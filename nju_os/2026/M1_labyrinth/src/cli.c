#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "cli.h"
#include "player.h"

static struct option long_options[] = {
    {"map", required_argument, 0, 'm'},
    {"player", required_argument, 0, 'p'},
    {"move", required_argument, 0, 0},
    {"version", no_argument, 0, 0},
    {0, 0, 0, 0}
};

static int count_map = 0;
static int count_player = 0;

int parse_argument(int argc, char *argv[], GameConfig *config) {
    int opt;
    int option_index = 0;
    while((opt = getopt_long(argc, argv, "m:p:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'm':
                config->map_name = optarg;
                count_map = 1;
                break;
            case 'p':
                if(optarg[1] != '\0') {
                    fprintf(stderr, "Error: Invalid player number\n");
                    return 1;
                }
                config->player_num = atoi(optarg);
                count_player = 1;
                break;
            case 0:
                if(strcmp(long_options[option_index].name, "move") == 0) {
                    config->direct_select = optarg;
                } else if (strcmp(long_options[option_index].name, "version") == 0) {
                    config->version = 1;
                }
                break;
            default:
                printf("Error: Invalid argument\n");
                return 1;
            }
    }

    return 0;
}

int validate_config(GameConfig *config, int argc) {
    if(config->version == 1) {
        if(argc != 2) {
            fprintf(stderr, "Error: --version requires no arguments\n");
            return 1;
        }
        return 0;
    } 
    if(config->version == 0 && (!count_player)) {
        fprintf(stderr, "Error: --player is required\n");
        return 1;
    }



    if(config->player_num > 9 || config->player_num < 0) {
        fprintf(stderr, "Error: Invalid player number \n");
        return 1;
    }

    if(config->direct_select != NULL) {
        if(strcmp(config->direct_select, "up") != 0 &&
            strcmp(config->direct_select, "down") != 0 &&
            strcmp(config->direct_select, "left") != 0 &&
            strcmp(config->direct_select, "right") != 0) {
                fprintf(stderr, "Error: Invalid direct select \n");
            return 1;
        }
    }

    return 0;
}