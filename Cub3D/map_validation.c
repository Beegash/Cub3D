#include "cub3d.h"
#include "./get_next_line/get_next_line.h"

static void handle_map_error(const char *message)
{
	fprintf(stderr, "Error: %s\n", message);
	exit(EXIT_FAILURE);
}

static void check_all_characters(t_game *game)
{
	for (int y = 0; game->map->map_line[y]; y++)
	{
		for (int x = 0; game->map->map_line[y][x]; x++)
		{
			char c = game->map->map_line[y][x];
			if (!strchr(" 01NSEW", c))
			{
				fprintf(stderr, "Error: Invalid character (ASCII %d) at (%d,%d)\n", c, x, y);
				exit(EXIT_FAILURE);
			}
		}
	}
}

void check_boundaries(t_game *game, int y, int x)
{

	if (y < 0 || x < 0 || game->map->map_line[y] == NULL || game->map->map_line[y][x] == '\0')
	{
		fprintf(stderr, "Error: Map is not closed/surrounded by walls (boundary reached).\n");
		exit(EXIT_FAILURE);
	}

	char c = game->map->map_line[y][x];
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
	{

		game->map->map_line[y][x] = 'V';

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

char **copy_map(char **map)
{
	int rows = 0;
	while (map[rows] != NULL) // Harita satırlarını say
		rows++;

	char **new_map = malloc((rows + 1) * sizeof(char *));
	if (!new_map)
	{
		perror("Error allocating memory for copy_map");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < rows; i++)
	{
		new_map[i] = ft_strdup(map[i]);
		if (!new_map[i]) // strdup başarısız olursa
		{
			perror("Error allocating memory for map row");
			while (i > 0)
				free(new_map[--i]); // Önceki tahsisleri temizle
			free(new_map);
			exit(EXIT_FAILURE);
		}
	}
	new_map[rows] = NULL; // Sonlandırıcı ekle
	return new_map;
}

char *replace_tabs_with_spaces(char *line, int spaces_per_tab)
{
	int i = 0, j = 0;
	int len = ft_strlen(line);
	char *new_line = malloc(len * spaces_per_tab + 1);

	while (line[i] != '\0')
	{
		if (line[i] == '\t')
		{

			for (int k = 0; k < spaces_per_tab; k++)
			{
				new_line[j++] = ' ';
			}
		}
		else
		{
			new_line[j++] = line[i];
		}
		i++;
	}

	new_line[j] = '\0';
	return new_line;
}

void flood_fill_area(char **map, int y, int x)
{
	if (y < 0 || x < 0 || map[y] == NULL || map[y][x] == '\0')
		return;

	char c = map[y][x];
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == '1')
	{
		map[y][x] = 'A';

		flood_fill_area(map, y, x + 1);
		flood_fill_area(map, y, x - 1);
		flood_fill_area(map, y + 1, x);
		flood_fill_area(map, y - 1, x);
	}
}

void print_map(char **map)
{
	printf("===== MAP =====\n");
	for (int y = 0; map[y] != NULL; y++)
	{
		printf("%s\n", map[y]);
	}
	printf("================\n");
}

void check_isolated_areas(t_game *game)
{

	char **temp_map = copy_map(game->map->map_line);
	if (!temp_map)
	{
		fprintf(stderr, "Error: Failed to copy map\n");
		exit(EXIT_FAILURE);
	}
	printf("DEBUG\n");
	flood_fill_area(temp_map, game->loc_py, game->loc_px);
	print_map(temp_map);
printf("DEBUG\n");
	for (int y = 0; temp_map[y] != NULL; y++) // NULL kontrolü ekledik
	{
		for (int x = 0; temp_map[y][x] != '\0'; x++)
		{
			char c = temp_map[y][x];
			if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == '1')
			{
				fprintf(stderr, "Error: Isolated area detected in the map!\n");
				free_map(temp_map);
				exit(EXIT_FAILURE);
			}
		}
	}
printf("DEBUG\n");
	free_map(temp_map);
	printf("DEBUG\n");
}
void validate_map(t_game *game)
{
	game->playercount = 0;
	check_all_characters(game);

	for (int y = 0; game->map->map_line[y]; y++)
	{
		for (int x = 0; game->map->map_line[y][x]; x++)
		{
			char c = game->map->map_line[y][x];
			if (strchr("NSEW", c))
			{
				game->playercount++;
				game->loc_px = x;
				game->loc_py = y;
				game->playertype = c;
			}
		}
	}

	if (game->playercount != 1)
		handle_map_error(game->playercount ? "Multiple players" : "No player");

	check_isolated_areas(game);
	printf("DEBUG\n");
	check_boundaries(game, game->loc_py, game->loc_px);
	printf("DEBUG\n");

	printf("DEBUG\n");

	for (int y = 0; game->map->map_line[y]; y++)
	{
		int len = strlen(game->map->map_line[y]);
		if (game->map->map_line[y][0] != '1' || game->map->map_line[y][len - 1] != '1')
			handle_map_error("Map borders not closed");
	}
}
void free_map(char **map)
{
	if (!map) // NULL kontrolü ekledik
		return;

	for (int i = 0; map[i] != NULL; i++)
		free(map[i]);

	free(map);
}

char *ft_strdup(const char *s1)
{
	size_t size;
	size_t i;
	char *str;

	i = 0;
	size = ft_strlen(s1);
	str = (char *)malloc(size + 1);
	if (!str)
		return (NULL);
	while (i < size)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char **read_map_from_file(char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	char **map_lines = malloc(sizeof(char *) * 1000);
	if (!map_lines)
	{
		perror("Error allocating memory for map");
		exit(EXIT_FAILURE);
	}

	int i = 0;
	char *line;
	while ((line = get_next_line(fd)) != NULL)
	{
		char *new_line = replace_tabs_with_spaces(line, 4);
		free(line);
		line = new_line;

		char *cleaned = ft_trimend(line, " \t\r\n");
		free(line);
		map_lines[i] = cleaned;
		i++;
	}
	map_lines[i] = NULL;

	close(fd);
	return map_lines;
}
