#ifndef CLI_H
#define CLI_H

typedef struct {
    char *map_name;
    int player_num;
    char *direct_select;
    int version;
}GameConfig;


int parse_argument(int argc, char *argv[], GameConfig *config);
int validate_config(GameConfig *config, int argc);

#endif