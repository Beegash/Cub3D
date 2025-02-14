/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 01:52:48 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/14 02:44:18 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	check_all_characters(t_game *game)
{
	char	c;
	int		x;
	int		y;

	y = 0;
	while (game->map->map_line[y])
	{
		x = 0;
		while (game->map->map_line[y][x])
		{
			c = game->map->map_line[y][x];
			if (!ft_strchr(" 01NSEW", c))
				handle_map_error(game, NULL, "Invalid character in map");
			x++;
		}
		y++;
	}
}

void	check_boundaries(t_game *game, int y, int x)
{
	char	**temp_map;

	temp_map = copy_map(game->map->map_line);
	if (!temp_map)
		handle_map_error(game, NULL, "Failed to copy map");
	recursive_check_boundaries(temp_map, y, x, game);
	free_map(temp_map);
}

static void	find_player(t_game *game, char c, int x, int y)
{
	if (ft_strchr("NSEW", c))
	{
		game->playercount++;
		game->loc_px = x;
		game->loc_py = y;
		game->playertype = c;
	}
}

static char	*get_player_error(int playercount)
{
	if (playercount > 1)
		return ("Multiple players");
	return ("No player");
}

void	validate_map(t_game *game)
{
	char	c;
	int		x;
	int		y;

	game->playercount = 0;
	check_all_characters(game);
	y = 0;
	while (game->map->map_line[y])
	{
		x = 0;
		while (game->map->map_line[y][x])
		{
			c = game->map->map_line[y][x];
			find_player(game, c, x, y);
			x++;
		}
		y++;
	}
	if (game->playercount != 1)
		handle_map_error(game, NULL, get_player_error(game->playercount));
	check_isolated_areas(game);
	check_boundaries(game, game->loc_py, game->loc_px);
}
