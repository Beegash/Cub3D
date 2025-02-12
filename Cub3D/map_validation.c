#include "cub3d.h"
#include "./get_next_line/get_next_line.h"

void flood_fill_area(char **map, int y, int x) {
    if (y < 0 || !map[y]) return;
    int row_len = ft_strlen(map[y]);
    if (x < 0 || x >= row_len) return;
    
    char c = map[y][x];
    if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == '1') {
        map[y][x] = 'A';
        flood_fill_area(map, y, x + 1);
        flood_fill_area(map, y, x - 1);
        flood_fill_area(map, y + 1, x);
        flood_fill_area(map, y - 1, x);
    }
}

void free_game_resources(t_game *game) {
    if (game->map) {
        if (game->map->map_line) free_map(game->map->map_line);
        free(game->map->north_texture);
        free(game->map->south_texture);
        free(game->map->west_texture);
        free(game->map->east_texture);
        free(game->map);
    }
}

static void handle_map_error(t_game *game, char **temp_map, const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    if (temp_map) free_map(temp_map);
    free_game_resources(game);
    exit(EXIT_FAILURE);
}

static void check_all_characters(t_game *game) {
    for (int y = 0; game->map->map_line[y]; y++) {
        for (int x = 0; game->map->map_line[y][x]; x++) {
            char c = game->map->map_line[y][x];
            if (!strchr(" 01NSEW", c))
                handle_map_error(game, NULL, "Invalid character in map");
        }
    }
}

static void recursive_check_boundaries(char **temp_map, int y, int x, t_game *game) {
    if (y < 0 || x < 0 || !temp_map[y] || x >= (int)ft_strlen(temp_map[y]))
        handle_map_error(game, temp_map, "Map is not closed/surrounded by walls");
    
    char c = temp_map[y][x];
    if (c == '0' || strchr("NSEW", c)) {
        temp_map[y][x] = 'V';
        recursive_check_boundaries(temp_map, y, x+1, game);
        recursive_check_boundaries(temp_map, y, x-1, game);
        recursive_check_boundaries(temp_map, y+1, x, game);
        recursive_check_boundaries(temp_map, y-1, x, game);
    } else if (c == ' ')
        handle_map_error(game, temp_map, "Map contains open space accessible from player area");
}

void check_boundaries(t_game *game, int y, int x) {
    char **temp_map = copy_map(game->map->map_line);
    if (!temp_map) handle_map_error(game, NULL, "Failed to copy map");
    
    recursive_check_boundaries(temp_map, y, x, game);
    free_map(temp_map);
}

void check_isolated_areas(t_game *game) {
    char **temp_map = copy_map(game->map->map_line);
    if (!temp_map) handle_map_error(game, NULL, "Failed to copy map");
    
    flood_fill_area(temp_map, game->loc_py, game->loc_px);
    
    for (int y = 0; temp_map[y]; y++) {
        for (int x = 0; temp_map[y][x]; x++) {
            char c = temp_map[y][x];
            if (c == '0' || strchr("NSEW", c))
                handle_map_error(game, temp_map, "Isolated area detected in the map!");
        }
    }
    free_map(temp_map);
}

char **read_map_from_file(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char **map_lines = NULL;
    char *line;
    int capacity = 100;
    int size = 0;
    
    map_lines = malloc(sizeof(char *) * capacity);
    if (!map_lines) {
        perror("Error allocating memory for map");
        close(fd);
        exit(EXIT_FAILURE);
    }

    while ((line = get_next_line(fd)) != NULL) {
        if (size >= capacity-1) {
            capacity *= 2;
            char **new_lines = realloc(map_lines, sizeof(char *) * capacity);
            if (!new_lines) {
                free(line);
                free_map(map_lines);
                close(fd);
                exit(EXIT_FAILURE);
            }
            map_lines = new_lines;
        }
        
        char *new_line = replace_tabs_with_spaces(line, 4);
        free(line);
        if (!new_line) {
            free_map(map_lines);
            close(fd);
            exit(EXIT_FAILURE);
        }

        char *cleaned = ft_trimend(new_line, " \t\r\n");
        free(new_line);
        if (!cleaned) {
            free_map(map_lines);
            close(fd);
            exit(EXIT_FAILURE);
        }

        map_lines[size++] = cleaned;
    }
    map_lines[size] = NULL;
    close(fd);
    return map_lines;
}

void validate_map(t_game *game) {
    game->playercount = 0;
    check_all_characters(game);

    for (int y = 0; game->map->map_line[y]; y++) {
        for (int x = 0; game->map->map_line[y][x]; x++) {
            char c = game->map->map_line[y][x];
            if (strchr("NSEW", c)) {
                game->playercount++;
                game->loc_px = x;
                game->loc_py = y;
                game->playertype = c;
            }
        }
    }

    if (game->playercount != 1)
        handle_map_error(game, NULL, game->playercount ? "Multiple players" : "No player");

    check_isolated_areas(game);
    check_boundaries(game, game->loc_py, game->loc_px);

    for (int y = 0; game->map->map_line[y]; y++) {
        int len = ft_strlen(game->map->map_line[y]);
        if (game->map->map_line[y][0] != '1' || game->map->map_line[y][len-1] != '1')
            handle_map_error(game, NULL, "Map borders not closed");
    }
}