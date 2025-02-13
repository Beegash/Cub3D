/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iozmen <iozmen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:48:46 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/13 20:54:22 by iozmen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>

int	error_message(char *str, int err)
{
	printf("Error: %s\n", str);
	return (err);
}

int	control_extension(char *argv)
{
	int	len;

	len = ft_strlen(argv);
	if (len < 4)
		return (0);
	if (argv[len - 4] != '.' || argv[len - 3] != 'c' || argv[len - 2] != 'u'
		|| argv[len - 1] != 'b')
		return (0);
	return (1);
}

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

void	handle_rotation(t_game *game, double rot_speed)
{
	double	old_dir_x;
	double	old_plane_x;

	if (game->key->right)
	{
		old_dir_x = game->player->dir_x;
		game->player->dir_x = game->player->dir_x * cos(-rot_speed)
			- game->player->dir_y * sin(-rot_speed);
		game->player->dir_y = old_dir_x * sin(-rot_speed) + game->player->dir_y
			* cos(-rot_speed);
		old_plane_x = game->player->plane_x;
		game->player->plane_x = game->player->plane_x * cos(-rot_speed)
			- game->player->plane_y * sin(-rot_speed);
		game->player->plane_y = old_plane_x * sin(-rot_speed)
			+ game->player->plane_y * cos(-rot_speed);
	}
	if (game->key->left)
	{
		old_dir_x = game->player->dir_x;
		game->player->dir_x = game->player->dir_x * cos(rot_speed)
			- game->player->dir_y * sin(rot_speed);
		game->player->dir_y = old_dir_x * sin(rot_speed) + game->player->dir_y
			* cos(rot_speed);
		old_plane_x = game->player->plane_x;
		game->player->plane_x = game->player->plane_x * cos(rot_speed)
			- game->player->plane_y * sin(rot_speed);
		game->player->plane_y = old_plane_x * sin(rot_speed)
			+ game->player->plane_y * cos(rot_speed);
	}
}

int	game_loop(t_game *game)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 0.1;
	rot_speed = 0.05;
	handle_forward_backward(game, move_speed);
	handle_left_right(game, move_speed);
	handle_rotation(game, rot_speed);
	ft_memset(game->mlx_data, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
	perform_raycasting(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img_ptr, 0, 0);
	return (0);
}

int	check_single_texture(char *texture_path, char *direction)
{
	int	fd;

	fd = open(texture_path, O_RDONLY);
	if (fd < 0)
	{
		printf("Hata: %s texture dosyası bulunamadı: %s\n", direction,
			texture_path);
		return (0);
	}
	close(fd);
	return (1);
}

int	check_textures(t_map *map)
{
	if (!map->north_texture || !map->south_texture || !map->west_texture
		|| !map->east_texture)
	{
		printf("Hata: Eksik texture yolu\n");
		return (0);
	}
	if (!check_single_texture(map->north_texture, "North")
		|| !check_single_texture(map->south_texture, "South")
		|| !check_single_texture(map->west_texture, "West")
		|| !check_single_texture(map->east_texture, "East"))
		return (0);
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

char	**prepare_map_data(char **all_lines, int map_start)
{
	int		i;
	char	**actual_map;

	i = 0;
	while (all_lines[map_start + i])
		i++;
	actual_map = malloc(sizeof(char *) * (i + 1));
	if (!actual_map)
		return (NULL);
	i = 0;
	while (all_lines[map_start + i])
	{
		actual_map[i] = ft_strdup(all_lines[map_start + i]);
		i++;
	}
	actual_map[i] = NULL;
	return (actual_map);
}

void	start_game_loop(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop_hook(game->mlx, game_loop, game);
	mlx_loop(game->mlx);
}

void	cleanup_resources(t_game *game, char **all_lines, char **actual_map)
{
	if (all_lines)
		free_map(all_lines);
	if (actual_map && actual_map != game->map->map_line)
		free_map(actual_map);
	if (!game)
		return ;
	cleanup_map_content(game->map);
	if (game->map)
		free(game->map);
	cleanup_mlx_content(game);
	if (game->img)
		free(game->img);
	if (game->key)
		free(game->key);
	if (game->player)
		free(game->player);
	if (game->raycast)
		free(game->raycast);
	if (game->mapname)
		free(game->mapname);
	if (game)
		free(game);
}

int	init_map_and_textures(t_game *game, char *map_file, char ***all_lines,
		int *map_start)
{
	*all_lines = read_map_from_file(map_file, game);
	if (!*all_lines)
		return (0);
	*map_start = get_texture(*all_lines, game->map);
	if (!*map_start || !check_textures(game->map))
	{
		free_map(*all_lines);
		*all_lines = NULL;
		return (0);
	}
	return (1);
}

int	setup_game_map(t_game *game, char **all_lines, int map_start,
		char ***actual_map)
{
	*actual_map = prepare_map_data(all_lines, map_start);
	if (!*actual_map)
		return (0);
	game->map->map_line = *actual_map;
	validate_map(game);
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
		return (0);
	}
	return (1);
}

int	initialize_and_validate(t_game **game, char *map_file)
{
	char	**all_lines;
	char	**actual_map;

	if (!(*game = init_game_s()))
		return (0);
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

int	main(int argc, char **argv)
{
	t_game *game;

	if (argc != 2)
		return (error_message("Wrong argument count\n", 1));
	if (!control_extension(argv[1]))
		return (error_message("Invalid file extension\n", 1));

	if (!initialize_and_validate(&game, argv[1]))
		return (error_message("Game initialization failed\n", 1));

	start_game_loop(game);
	cleanup_all(game);
	return (0);
}