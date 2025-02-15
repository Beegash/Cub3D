/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_start.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 01:33:56 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/15 01:59:19 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	game_loop(t_game *game)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 0.05;
	rot_speed = 0.03;
	handle_forward_backward(game, move_speed);
	handle_left_right(game, move_speed);
	handle_rotation(game, rot_speed);
	ft_memset(game->mlx_data, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));
	perform_raycasting(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img_ptr, 0, 0);
	return (0);
}

void	start_game_loop(t_game *game)
{
	mlx_hook(game->win, 2, 1L << 0, key_press, game);
	mlx_hook(game->win, 3, 1L << 1, key_release, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop_hook(game->mlx, game_loop, game);
	mlx_loop(game->mlx);
}

static	char	**prepare_map_data(char **all_lines, int map_start)
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

int	setup_game_map(t_game *game, char **all_lines, int map_start,
		char ***actual_map)
{
	*actual_map = prepare_map_data(all_lines, map_start);
	if (!*actual_map)
		return (error_message("Game initialization failed", 0));
	game->map->map_line = *actual_map;
	game->all_lines = all_lines;
	validate_map(game);
	return (1);
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
