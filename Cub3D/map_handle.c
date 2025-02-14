/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 02:59:01 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/14 02:59:25 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_malloc_error(char **new_map, int i, char *error_msg)
{
	perror(error_msg);
	while (i > 0)
		free(new_map[--i]);
	free(new_map);
	exit(EXIT_FAILURE);
}

void	hmerror(t_game *game, char **temp_map, const char *message)
{
	free_map(game->all_lines);
	printf("Error\n%s\n", message);
	if (temp_map)
	{
		free_map(temp_map);
	}
	if (game)
		cleanup_all(game);
	exit(0);
}
