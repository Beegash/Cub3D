/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:12:27 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/14 03:16:22 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	init_single_texture(t_game *game, int i, char *texture_path)
{
	game->walls[i] = malloc(sizeof(t_textures));
	if (!game->walls[i])
		return (error_message("Memory allocation failed", 0));
	game->walls[i]->img = mlx_xpm_file_to_image(game->mlx, texture_path,
			&game->walls[i]->w, &game->walls[i]->h);
	if (!game->walls[i]->img)
		return (error_message("Texture loading failed", 0));
	game->walls[i]->addr = (int *)mlx_get_data_addr(game->walls[i]->img,
			&game->walls[i]->bpp, &game->walls[i]->line_len,
			&game->walls[i]->endian);
	return (1);
}

static int	load_textures(t_game *game)
{
	int		i;
	char	*texture_paths[4];

	texture_paths[0] = game->map->north_texture;
	texture_paths[1] = game->map->south_texture;
	texture_paths[2] = game->map->west_texture;
	texture_paths[3] = game->map->east_texture;
	i = 0;
	while (i < 4)
	{
		if (!init_single_texture(game, i, texture_paths[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (error_message("MLX initialization failed", 0));
	game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
	if (!game->win)
		return (error_message("Window creation failed", 0));
	game->img_ptr = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!game->img_ptr)
		return (error_message("Image creation failed", 0));
	game->mlx_data = (int *)mlx_get_data_addr(game->img_ptr, &game->pixel_bits,
			&game->line_bytes, &game->endian);
	return (1);
}

int	init_game(t_game *game)
{
	init_game_struct(game);
	if (!init_mlx(game))
		return (0);
	if (!load_textures(game))
		return (error_message("Game initialization failed", 0));
	return (1);
}
