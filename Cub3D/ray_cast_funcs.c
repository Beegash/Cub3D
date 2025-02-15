/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_funcs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:44:12 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/14 03:37:04 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

void	calculate_wall_height(t_game *game, t_raycast *ray)
{
	(void)game;
	if (ray->side1 == 0)
		ray->perp_dist = (ray->sidedist_x - ray->deltadist_x);
	else
		ray->perp_dist = (ray->sidedist_y - ray->deltadist_y);
	ray->height = (int)(WINDOW_HEIGHT / (ray->perp_dist * 1.5));
	ray->start_y = -ray->height / 2 + WINDOW_HEIGHT / 2;
	if (ray->start_y < 0)
		ray->start_y = 0;
	ray->end_y = ray->height / 2 + WINDOW_HEIGHT / 2;
	if (ray->end_y >= WINDOW_HEIGHT)
		ray->end_y = WINDOW_HEIGHT;
}

static void	set_ray_steps(t_game *game, t_raycast *ray, int map_x, int map_y)
{
	if (ray->raydir_x < 0)
	{
		ray->step_x = -1;
		ray->sidedist_x = (game->player->x - map_x) * ray->deltadist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->sidedist_x = (map_x + 1.0 - game->player->x) * ray->deltadist_x;
	}
	if (ray->raydir_y < 0)
	{
		ray->step_y = -1;
		ray->sidedist_y = (game->player->y - map_y) * ray->deltadist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->sidedist_y = (map_y + 1.0 - game->player->y) * ray->deltadist_y;
	}
}

void	calculate_ray_dir(t_game *game, t_raycast *ray, int x)
{
	int	map_x;
	int	map_y;

	ray->camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	ray->raydir_x = game->player->dir_x + game->player->plane_x * ray->camera_x;
	ray->raydir_y = game->player->dir_y + game->player->plane_y * ray->camera_x;
	ray->deltadist_x = fabs(1 / ray->raydir_x);
	ray->deltadist_y = fabs(1 / ray->raydir_y);
	map_x = (int)game->player->x;
	map_y = (int)game->player->y;
	set_ray_steps(game, ray, map_x, map_y);
}

void	perform_dda(t_game *game, t_raycast *ray)
{
	int	map_x;
	int	map_y;

	map_x = (int)game->player->x;
	map_y = (int)game->player->y;
	ray->hit = 0;
	while (ray->hit == 0)
	{
		if (ray->sidedist_x < ray->sidedist_y)
		{
			ray->sidedist_x += ray->deltadist_x;
			map_x += ray->step_x;
			ray->side1 = 0;
		}
		else
		{
			ray->sidedist_y += ray->deltadist_y;
			map_y += ray->step_y;
			ray->side1 = 1;
		}
		if (game->map->map_line[map_y][map_x] == '1')
			ray->hit = 1;
	}
}

void	perform_raycasting(t_game *game)
{
	int	x;

	draw_floor_ceiling(game);
	x = 0;
	while (x < WINDOW_WIDTH)
	{
		calculate_ray_dir(game, game->raycast, x);
		perform_dda(game, game->raycast);
		calculate_wall_height(game, game->raycast);
		draw_wall(game, x, game->raycast);
		x++;
	}
}
