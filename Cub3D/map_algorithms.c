/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_algorithms.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iozmen <iozmen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:50:55 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/14 19:07:19 by iozmen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	flood_fill_area(char **map, int y, int x, t_game *game)
{
	int		row_len;
	char	c;

	if (y < 0 || !map[y])
		return ;
	row_len = ft_strlen(map[y]);
	if (x < 0 || x >= row_len)
		return ;
	c = map[y][x];
	if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == '1')
	{
		if ((c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
			&& (y == 0 || !map[y + 1] || x == 0 || x == row_len - 1 || map[y][x
				+ 1] == ' ' || map[y][x - 1] == ' ' || map[y + 1][x] == ' '
				|| map[y - 1][x] == ' '))
			hmerror(game, map, "Open space or not closed");
		map[y][x] = 'A';
		flood_fill_area(map, y, x + 1, game);
		flood_fill_area(map, y, x - 1, game);
		flood_fill_area(map, y + 1, x, game);
		flood_fill_area(map, y - 1, x, game);
	}
}

void	recursive_check_boundaries(char **temp_map, int y, int x, t_game *game)
{
	char	c;

	if (y < 0 || x < 0 || !temp_map[y] || x >= (int)ft_strlen(temp_map[y]))
		hmerror(game, temp_map, "Map is not closed/surrounded by walls");
	c = temp_map[y][x];
	if (c == '0' || ft_strchr("NSEW", c))
	{
		temp_map[y][x] = 'V';
		recursive_check_boundaries(temp_map, y, x + 1, game);
		recursive_check_boundaries(temp_map, y, x - 1, game);
		recursive_check_boundaries(temp_map, y + 1, x, game);
		recursive_check_boundaries(temp_map, y - 1, x, game);
	}
	else if (c == ' ')
		hmerror(game, temp_map, "Map contains open space ");
}

void	check_isolated_areas(t_game *game)
{
	char	**temp_map;
	char	c;
	int		x;
	int		y;

	temp_map = copy_map(game->map->map_line);
	if (!temp_map)
		hmerror(game, NULL, "Failed to copy map");
	flood_fill_area(temp_map, game->loc_py, game->loc_px, game);
	y = 0;
	while (temp_map[y])
	{
		x = 0;
		while (temp_map[y][x])
		{
			c = temp_map[y][x];
			if (c == '0' || ft_strchr("NSEW", c))
			{
				hmerror(game, temp_map, "Isolated area detected in the map!");
			}
			x++;
		}
		y++;
	}
	free_map(temp_map);
}

void	free_game_resources(t_game *game)
{
	if (game->map)
	{
		if (game->map->map_line)
			free_map(game->map->map_line);
		free(game->map->north_texture);
		free(game->map->south_texture);
		free(game->map->west_texture);
		free(game->map->east_texture);
		free(game->map);
	}
}

char	**init_map_lines(int fd, t_game *game)
{
	char	**map_lines;

	map_lines = malloc(sizeof(char *) * 100);
	if (!map_lines)
	{
		perror("Error allocating memory for map");
		close(fd);
		cleanup_all(game);
		exit(EXIT_FAILURE);
	}
	return (map_lines);
}
