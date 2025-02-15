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

int	is_open_space_or_not_closed(char **map, int y, int x, int row_len)
{
	if (y == 0 || !map[y + 1] || x == 0 || x == row_len - 1
		|| map[y][x + 1] == ' ' || map[y][x - 1] == ' '
		|| map[y + 1][x] == ' ' || map[y - 1][x] == ' '
		|| map[y][x + 1] == '\0' || map[y][x - 1] == '\0'
		|| map[y + 1][x] == '\0' || map[y - 1][x] == '\0')
		return (1);
	return (0);
}

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
			&& is_open_space_or_not_closed(map, y, x, row_len))
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
			if (c == '0' || ft_strchr("NSEW", c) || c == '1')
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
		free(game->map->n_text);
		free(game->map->s_text);
		free(game->map->w_text);
		free(game->map->e_text);
		free(game->map);
	}
}
