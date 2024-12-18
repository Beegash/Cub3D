#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	char **map_line;
} t_map;

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
		fprintf(stderr, "Error: Invalid character '%c' in accessible map area.\n", c);
		exit(EXIT_FAILURE);
	}
}

void validate_map(t_map *game)
{
	int player_x = -1, player_y = -1, player_count = 0;

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
}

char **copy_map(char **map)
{
	int rows = 0;
	while (map[rows] != NULL)
		rows++;

	char **new_map = malloc((rows + 1) * sizeof(char *));
	for (int i = 0; i < rows; i++)
	{
		new_map[i] = strdup(map[i]);
	}
	new_map[rows] = NULL;
	return new_map;
}

void free_map(char **map)
{
	for (int i = 0; map[i] != NULL; i++)
		free(map[i]);
	free(map);
}

int main()
{
	char *map_lines[] = {
		"   1111111111111111111111111",
		"  11000000000110000000000001                 11111111111",
		"111000000001100000000000011                 111111a1111",
		"1000000000110000000000001                 11111101111",
		"11111000011100000000000011111",
		"     100100000000000000000000111111111",
		"111111000011000001110000000000001",
		"100000000011000001110111100111111",
		"11110111111111011100000010001",
		"11110111111111011101010010001",
		"11000000110101011100000010001",
		"10000000000000001100000010001",
		"1110000000000011010100100011",
		"  110011111101010111110111100111",
		"11110S111 1110101 111111110001",
		"111111111 11111111111111111111",

		NULL};

	t_map game;
	game.map_line = copy_map(map_lines);

	printf("Harita doğrulaması başlıyor...\n");
	validate_map(&game);

	printf("Harita doğrulandı. Güncellenmiş harita:\n");
	for (int i = 0; game.map_line[i] != NULL; i++)
	{
		printf("%s\n", game.map_line[i]);
	}

	free_map(game.map_line);
	return 0;
}