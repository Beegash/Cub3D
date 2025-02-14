/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_draw.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:33:35 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/14 03:38:20 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_texture_number(t_raycast *ray)
{
	if (ray->side1 == 0)
	{
		if (ray->raydir_x > 0)
			return (3);
		return (2);
	}
	if (ray->raydir_y > 0)
		return (1);
	return (0);
}

static void	calculate_texture_position(t_game *game, t_raycast *ray,
		int tex_num, double *wall_x)
{
	if (ray->side1 == 0)
		*wall_x = game->player->y + ray->perp_dist * ray->raydir_y;
	else
		*wall_x = game->player->x + ray->perp_dist * ray->raydir_x;
	*wall_x -= floor(*wall_x);
	ray->tex_x = (int)(*wall_x * game->walls[tex_num]->w);
	if ((ray->side1 == 0 && ray->raydir_x > 0)
		|| (ray->side1 == 1 && ray->raydir_y < 0))
		ray->tex_x = game->walls[tex_num]->w - ray->tex_x - 1;
}

static void	draw_vertical_line(t_game *game, t_raycast *ray,
		int tex_num, int x)
{
	double	step;
	double	tex_pos;
	int		y;
	int		color;

	step = 1.0 * game->walls[tex_num]->h / ray->height;
	tex_pos = (ray->start_y - WINDOW_HEIGHT / 2 + ray->height / 2) * step;
	y = ray->start_y;
	while (y < ray->end_y)
	{
		ray->tex_y = (int)tex_pos & (game->walls[tex_num]->h - 1);
		tex_pos += step;
		color = game->walls[tex_num]->addr[game->walls[tex_num]->h
			* ray->tex_y + ray->tex_x];
		if (ray->side1 == 1)
			color = (color >> 1) & 8355711;
		if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
			game->mlx_data[y * WINDOW_WIDTH + x] = color;
		y++;
	}
}

void	draw_wall(t_game *game, int x, t_raycast *ray)
{
	int		tex_num;
	double	wall_x;

	tex_num = get_texture_number(ray);
	calculate_texture_position(game, ray, tex_num, &wall_x);
	draw_vertical_line(game, ray, tex_num, x);
}
