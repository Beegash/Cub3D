/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iozmen <iozmen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:47:05 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/14 15:21:52 by iozmen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./get_next_line/get_next_line.h"
#include "cub3d.h"

char	*texture_path(char *line, int j)
{
	char	*full_line;
	char	*trimmed;

	while (line[j] && (line[j] == '\t' || line[j] == ' '))
		j++;
	full_line = ft_strdup(line + j);
	if (!full_line)
		return (NULL);
	trimmed = ft_strtrim(full_line);
	free(full_line);
	if (!trimmed)
		return (NULL);
	return (trimmed);
}

static int	handle_texture_identifier(char *line, int j, t_map *map)
{
	if (line[j] == 'N' && line[j + 1] == 'O' && line[j + 2] == ' '
		&& !(map->north_texture))
		map->north_texture = texture_path(line, j + 2);
	else if (line[j] == 'S' && line[j + 1] == 'O' && line[j + 2] == ' '
		&& !(map->south_texture))
		map->south_texture = texture_path(line, j + 2);
	else if (line[j] == 'W' && line[j + 1] == 'E' && line[j + 2] == ' '
		&& !(map->west_texture))
		map->west_texture = texture_path(line, j + 2);
	else if (line[j] == 'E' && line[j + 1] == 'A' && line[j + 2] == ' '
		&& !(map->east_texture))
		map->east_texture = texture_path(line, j + 2);
	return (1);
}

static int	handle_color_identifier(char *line, int j, t_map *map)
{
	if (line[j] == 'F' && line[j + 1] == ' ' && !(map->floor_color[0]))
	{
		j++;
		if (!rgb_numbers(line, j, map->floor_color))
			return (3);
	}
	else if (line[j] == 'C' && line[j+ 1] == ' ' && !(map->ceiling_color[0]))
	{
		j++;
		if (!rgb_numbers(line, j, map->ceiling_color))
			return (3);
	}
	return (1);
}

static int	process_map_line(char *line, int j, t_map *map)
{
	while (line[j] && (line[j] == '\t' || line[j] == ' ' || line[j] == '\n'))
		j++;
	if (!line[j])
		return (-1);
	if (line[j] == 'N' || line[j] == 'S' || line[j] == 'W' || line[j] == 'E')
		return (handle_texture_identifier(line, j, map));
	else if (line[j] == 'F' || line[j] == 'C')
		return (handle_color_identifier(line, j, map));
	else if (line[j] == '1' || line[j] == '0')
		return (0);
	else if (line[j] != '\0' && line[j] != '\n')
		return (-2);
	return (1);
}

int	get_texture(char **map_line, t_map *map)
{
	int	i;
	int	result;

	i = 0;
	while (map_line[i])
	{
		result = process_map_line(map_line[i], 0, map);
		if (result == 0)
			return (i);
		if (result == 3)
			return (0);
		if (result == -2)
			return (0);
		if (result == -1)
		{
			i++;
			continue ;
		}
		i++;
	}
	return (0);
}
