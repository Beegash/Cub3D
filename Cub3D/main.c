/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:48:46 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/15 01:49:21 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	error_message(char *str, int err)
{
	printf("Error\n%s\n", str);
	return (err);
}

int	check_single_texture(char *texture_path)
{
	int	fd;

	fd = open(texture_path, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

int	check_textures(t_map *map)
{
	if (!map->n_text || !map->s_text || !map->w_text
		|| !map->e_text)
		return (error_message("Texture Error", 0));
	if (!map->floor_color_set || !map->ceiling_color_set)
		return (error_message("Color Error", 0));
	if (!check_single_texture(map->n_text)
		|| !check_single_texture(map->s_text)
		|| !check_single_texture(map->w_text)
		|| !check_single_texture(map->e_text))
		return (error_message("Texture file is not found", 0));
	return (1);
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

int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc != 2)
		return (error_message("Wrong argument count", 1));
	if (!control_extension(argv[1]))
		return (error_message("Invalid file extension", 1));
	if (!initialize_and_validate(&game, argv[1]))
		return (1);
	start_game_loop(game);
	cleanup_all(game);
	return (0);
}
