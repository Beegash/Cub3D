/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_movement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 01:32:35 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/15 01:51:55 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_forward_backward(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	if (game->key->w)
	{
		new_x = game->player->x + game->player->dir_x * move_speed;
		new_y = game->player->y + game->player->dir_y * move_speed;
		if (game->map->map_line[(int)game->player->y][(int)new_x] != '1')
			game->player->x = new_x;
		if (game->map->map_line[(int)new_y][(int)game->player->x] != '1')
			game->player->y = new_y;
	}
	if (game->key->s)
	{
		new_x = game->player->x - game->player->dir_x * move_speed;
		new_y = game->player->y - game->player->dir_y * move_speed;
		if (game->map->map_line[(int)game->player->y][(int)new_x] != '1')
			game->player->x = new_x;
		if (game->map->map_line[(int)new_y][(int)game->player->x] != '1')
			game->player->y = new_y;
	}
}

void	handle_left_right(t_game *game, double move_speed)
{
	double	new_x;
	double	new_y;

	if (game->key->d)
	{
		new_x = game->player->x + game->player->plane_x * move_speed;
		new_y = game->player->y + game->player->plane_y * move_speed;
		if (game->map->map_line[(int)game->player->y][(int)new_x] != '1')
			game->player->x = new_x;
		if (game->map->map_line[(int)new_y][(int)game->player->x] != '1')
			game->player->y = new_y;
	}
	if (game->key->a)
	{
		new_x = game->player->x - game->player->plane_x * move_speed;
		new_y = game->player->y - game->player->plane_y * move_speed;
		if (game->map->map_line[(int)game->player->y][(int)new_x] != '1')
			game->player->x = new_x;
		if (game->map->map_line[(int)new_y][(int)game->player->x] != '1')
			game->player->y = new_y;
	}
}

static void	rotate_direction(t_game *game, double rot_speed, int direction)
{
	double	old_dir_x;
	double	old_plane_x;
	double	angle;

	angle = rot_speed;
	if (direction == -1)
		angle = -rot_speed;
	old_dir_x = game->player->dir_x;
	game->player->dir_x = game->player->dir_x * cos(angle)
		- game->player->dir_y * sin(angle);
	game->player->dir_y = old_dir_x * sin(angle)
		+ game->player->dir_y * cos(angle);
	old_plane_x = game->player->plane_x;
	game->player->plane_x = game->player->plane_x * cos(angle)
		- game->player->plane_y * sin(angle);
	game->player->plane_y = old_plane_x * sin(angle)
		+ game->player->plane_y * cos(angle);
}

void	handle_rotation(t_game *game, double rot_speed)
{
	if (game->key->right)
		rotate_direction(game, rot_speed, -1);
	if (game->key->left)
		rotate_direction(game, rot_speed, 1);
}
