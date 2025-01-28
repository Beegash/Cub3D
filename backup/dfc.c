#include "cub3d.h"
#include "get_next_line.h"

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
		new_map[i] = ft_strdup(map[i]);
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
char	*ft_strdup(const char *s1)
{
	size_t	size;
	size_t	i;
	char	*str;

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

		line = replace_tabs_with_spaces(line, 4);
		map_lines[i] = line;
		i++;
	}
	map_lines[i] = NULL;

	close(fd);
	return map_lines;
}
int main()
{

	char **map_lines = read_map_from_file("map.txt");

	t_map game;
	game.map_line = map_lines;

	printf("Map validation started...\n");
	validate_map(&game);

	printf("Map is valid. Printing map...\n");
	for (int i = 0; game.map_line[i] != NULL; i++)
	{
		printf("%s", game.map_line[i]);
	}

	free_map(game.map_line);
	return 0;
}
