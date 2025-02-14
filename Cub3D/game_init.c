/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 01:18:28 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/15 01:56:38 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	init_map_and_textures(t_game *game, char *map_file, char ***all_lines,
		int *map_start)
{
	*all_lines = read_map_from_file(map_file, game);
	if (!*all_lines)
		return (error_message("Game initialization failed", 0));
	*map_start = get_texture(*all_lines, game->map);
	if (!*map_start)
	{
		free_map(*all_lines);
		*all_lines = NULL;
		return (error_message("Element's format error", 0));
	}
	if (!check_textures(game->map))
	{
		free_map(*all_lines);
		*all_lines = NULL;
		return (0);
	}
	return (1);
}

t_game	*init_game_s(void)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	if (!game)
		return (NULL);
	ft_memset(game, 0, sizeof(t_game));
	game->map = malloc(sizeof(t_map));
	if (!game->map)
	{
		free(game);
		return (NULL);
	}
	ft_memset(game->map, 0, sizeof(t_map));
	return (game);
}

int	initialize_and_validate(t_game **game, char *map_file)
{
	char	**all_lines;
	char	**actual_map;

	*game = init_game_s();
	if (!(*game))
		return (error_message("Game initialization failed", 0));
	if (!init_and_setup_map(game, map_file, &all_lines, &actual_map))
		return (0);
	free_map(all_lines);
	if (!init_game(*game))
	{
		cleanup_resources(*game, NULL, actual_map);
		return (0);
	}
	return (1);
}

int	init_and_setup_map(t_game **game, char *map_file, char ***all_lines,
		char ***actual_map)
{
	int	map_start;

	if (!init_map_and_textures(*game, map_file, all_lines, &map_start))
	{
		cleanup_resources(*game, *all_lines, NULL);
		return (0);
	}
	if (!setup_game_map(*game, *all_lines, map_start, actual_map))
	{
		cleanup_resources(*game, *all_lines, NULL);
		return (error_message("Game initialization failed", 0));
	}
	return (1);
}
