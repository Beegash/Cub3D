/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:43:29 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/15 01:07:54 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_player_direction(t_game *game, t_direction dir)
{
	game->player->dir_x = dir.dir_x;
	game->player->dir_y = dir.dir_y;
	game->player->plane_x = dir.plane_x;
	game->player->plane_y = dir.plane_y;
}

static void	init_player_direction(t_game *game)
{
	t_direction	dir;

	if (game->playertype == 'N')
	{
		dir = (t_direction){0, -1, 0.66, 0};
		set_player_direction(game, dir);
	}
	else if (game->playertype == 'S')
	{
		dir = (t_direction){0, 1, -0.66, 0};
		set_player_direction(game, dir);
	}
	else if (game->playertype == 'E')
	{
		dir = (t_direction){1, 0, 0, 0.66};
		set_player_direction(game, dir);
	}
	else if (game->playertype == 'W')
	{
		dir = (t_direction){-1, 0, 0, -0.66};
		set_player_direction(game, dir);
	}
}

static void	init_key_states(t_game *game)
{
	game->key->w = 0;
	game->key->a = 0;
	game->key->s = 0;
	game->key->d = 0;
	game->key->left = 0;
	game->key->right = 0;
}

static void	init_basic_components(t_game *game)
{
	int	i;

	i = 0;
	game->mlx = NULL;
	game->win = NULL;
	game->img_ptr = NULL;
	game->mlx_data = NULL;
	while (i < 4)
	{
		game->walls[i] = NULL;
		i++;
	}
	game->player = malloc(sizeof(t_player));
	game->raycast = malloc(sizeof(t_raycast));
	game->key = malloc(sizeof(t_keycode));
	if (!game->player || !game->raycast || !game->key)
		exit(error_message("Memory allocation failed", 1));
	game->player->x = (double)game->loc_px + 0.5;
	game->player->y = (double)game->loc_py + 0.5;
	ft_memset(game->raycast, 0, sizeof(t_raycast));
}

void	init_game_struct(t_game *game)
{
	init_basic_components(game);
	init_player_direction(game);
	init_key_states(game);
}
