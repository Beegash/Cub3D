#include "cub3d.h"
#include "get_next_line.h"

void validate_texture(char *line, char *identifier, t_map *game);
void validate_color(char *line, char *identifier, int *color);
int is_valid_color(char *line);
int is_valid_texture_path(char *line);
void parse_cub_file(char *filename, t_map *game);

void validate_texture(char *line, char *identifier, t_map *game)
{
	if (strncmp(line, identifier, ft_strlen(identifier)) != 0)
	{
		fprintf(stderr, "Error: Invalid texture identifier.\n");
		exit(EXIT_FAILURE);
	}

	char *path = line + ft_strlen(identifier);
	while (*path == ' ')
		path++;
	if (!is_valid_texture_path(path))
	{
		fprintf(stderr, "Error: Invalid texture path.\n");
		exit(EXIT_FAILURE);
	}

	if (identifier[0] == 'N')
		game->north_texture = strdup(path);
	else if (identifier[0] == 'S')
		game->south_texture = strdup(path);
	else if (identifier[0] == 'W')
		game->west_texture = strdup(path);
	else if (identifier[0] == 'E')
		game->east_texture = strdup(path);
}

void validate_color(char *line, char *identifier, int *color)
{
	if (strncmp(line, identifier, ft_strlen(identifier)) != 0)
	{
		fprintf(stderr, "Error: Invalid color identifier.\n");
		exit(EXIT_FAILURE);
	}

	char *color_values = line + ft_strlen(identifier);
	while (*color_values == ' ')
		color_values++;

	if (!is_valid_color(color_values))
	{
		fprintf(stderr, "Error: Invalid color format.\n");
		exit(EXIT_FAILURE);
	}

	sscanf(color_values, "%d,%d,%d", &color[0], &color[1], &color[2]);
	if (color[0] < 0 || color[0] > 255 || color[1] < 0 || color[1] > 255 || color[2] < 0 || color[2] > 255)
	{
		fprintf(stderr, "Error: Color values out of range [0, 255].\n");
		exit(EXIT_FAILURE);
	}
}

int is_valid_color(char *line)
{
	int i = 0;
	while (line[i] != '\0' && line[i] != '\n')
	{
		if ((line[i] < '0' || line[i] > '9') && line[i] != ',' && line[i] != ' ')
			return 0;
		i++;
	}
	return 1;
}

int is_valid_texture_path(char *line)
{
	if (line[0] == '.' && line[1] == '/')
		return 1;
	return 0;
}

void parse_cub_file(char *filename, t_map *game)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	char *line;
	while ((line = get_next_line(fd)) != NULL)
	{

		if (strncmp(line, "NO", 2) == 0)
			validate_texture(line, "NO", game);
		else if (strncmp(line, "SO", 2) == 0)
			validate_texture(line, "SO", game);
		else if (strncmp(line, "WE", 2) == 0)
			validate_texture(line, "WE", game);
		else if (strncmp(line, "EA", 2) == 0)
			validate_texture(line, "EA", game);
		else if (strncmp(line, "F", 1) == 0)
			validate_color(line, "F", game->floor_color);
		else if (strncmp(line, "C", 1) == 0)
			validate_color(line, "C", game->ceiling_color);
		else if (line[0] != '\0')
		{

			game->map_line = read_map_from_file(line);
		}

		free(line);
	}

	close(fd);
}