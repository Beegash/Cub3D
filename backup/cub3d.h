#ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

typedef struct
{
	char **map_line;
	char *north_texture;
	char *south_texture;
	char *west_texture;
	char *east_texture;
	int floor_color[3];
	int ceiling_color[3];
} t_map;

void free_map(char **map);
char **read_map_from_file(char *filename);
void validate_map(t_map *game);
void check_boundaries(t_map *game, int y, int x);
char **copy_map(char **map);
char *replace_tabs_with_spaces(char *line, int spaces_per_tab);
char *ft_strdup(const char *s1);

#endif