/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast_draw_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iozmen <iozmen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:35:57 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/14 14:38:23 by iozmen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	get_color(int *color)
{
	return ((color[0] << 16) | (color[1] << 8) | color[2]);
}

void	draw_floor_ceiling(t_game *game)
{
	int	x;
	int	y;
	int	color;

	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			if (y < WINDOW_HEIGHT / 2)
				color = get_color(game->map->ceiling_color);
			else
				color = get_color(game->map->floor_color);
			game->mlx_data[y * WINDOW_WIDTH + x] = color;
			x++;
		}
		y++;
	}
}
