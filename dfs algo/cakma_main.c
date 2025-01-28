#include "cub3d.h"
#include "get_next_line.h"

// Harita kenarlarını kontrol etme fonksiyonu
void check_boundaries(t_map *game, int y, int x)
{
    if (y < 0 || x < 0 || game->map_line[y] == NULL || game->map_line[y][x] == '\0')
    {
        fprintf(stderr, "Error: Map is not closed/surrounded by walls (boundary reached).\n");
        exit(EXIT_FAILURE);
    }

    char c = game->map_line[y][x];
    if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
    {
        game->map_line[y][x] = 'V';

        check_boundaries(game, y, x + 1);
        check_boundaries(game, y, x - 1);
        check_boundaries(game, y + 1, x);
        check_boundaries(game, y - 1, x);
    }
    else if (c == ' ')
    {
        fprintf(stderr, "Error: Map contains open space accessible from the player's area.\n");
        exit(EXIT_FAILURE);
    }
    else if (c != '1' && c != 'V')
    {
        fprintf(stderr, "Error: Invalid character '-%c-' in accessible map area.\n", c);
        exit(EXIT_FAILURE);
    }
}

// Harita doğrulama fonksiyonu
void validate_map(t_map *game)
{
    int player_x = -1, player_y = -1, player_count = 0;

    // Harita üzerindeki her bir hücreyi kontrol etme
    for (int y = 0; game->map_line[y] != NULL; y++)
    {
        for (int x = 0; game->map_line[y][x] != '\0'; x++)
        {
            char c = game->map_line[y][x];
            if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
            {
                player_count++;
                if (player_count == 1)
                {
                    player_x = x;
                    player_y = y;
                }
            }
        }
    }

    if (player_count == 0)
    {
        fprintf(stderr, "Error: No player start position found.\n");
        exit(EXIT_FAILURE);
    }
    else if (player_count > 1)
    {
        fprintf(stderr, "Error: Multiple player start positions found.\n");
        exit(EXIT_FAILURE);
    }

    check_boundaries(game, player_y, player_x);

    // Dokuların geçerliliğini kontrol etme
    if (game->north_texture == NULL || game->south_texture == NULL || 
        game->west_texture == NULL || game->east_texture == NULL)
    {
        fprintf(stderr, "Error: Missing textures for one or more directions (N, S, E, W).\n");
        exit(EXIT_FAILURE);
    }

    // Renklerin geçerli olup olmadığını kontrol etme
    for (int i = 0; i < 3; i++)
    {
        if (game->floor_color[i] < 0 || game->floor_color[i] > 255 ||
            game->ceiling_color[i] < 0 || game->ceiling_color[i] > 255)
        {
            fprintf(stderr, "Error: Invalid color value for floor or ceiling. Each component should be between 0 and 255.\n");
            exit(EXIT_FAILURE);
        }
    }
}

int main()
{
    // Harita dosyasından veriyi okuma ve haritayı yükleme
    char **map_lines = read_map_from_file("map.cub");

    // t_map yapılandırmasını başlatma
    t_map game;
    game.map_line = map_lines;
    game.north_texture = "north_texture.xpm"; // Örnek doku yolu
    game.south_texture = "south_texture.xpm";
    game.west_texture = "west_texture.xpm";
    game.east_texture = "east_texture.xpm";
    game.floor_color[0] = 255; // Örnek renk
    game.floor_color[1] = 255;
    game.floor_color[2] = 255;
    game.ceiling_color[0] = 0;
    game.ceiling_color[1] = 0;
    game.ceiling_color[2] = 0;

    printf("Map validation started...\n");
    validate_map(&game);

    printf("Map is valid. Printing map...\n");
    for (int i = 0; game.map_line[i] != NULL; i++)
    {
        printf("%s", game.map_line[i]);
    }

    // Hafızayı temizle
    free_map(game.map_line);
    return 0;
}