/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 02:06:57 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/14 02:40:36 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	handle_file_error(int fd, t_game *game)
{
	perror("Error opening file");
	close(fd);
	cleanup_all(game);
	exit(EXIT_FAILURE);
}

static char	**resize_map_array(char **map_lines, char *line, int capacity)
{
	char	**new_lines;

	new_lines = realloc(map_lines, sizeof(char *) * capacity);
	if (!new_lines)
	{
		free(line);
		free_map(map_lines);
		return (NULL);
	}
	return (new_lines);
}

static char	*process_line(char *line, char **map_lines, int fd)
{
	char	*new_line;
	char	*cleaned;

	new_line = replace_tabs_with_spaces(line, 4);
	free(line);
	if (!new_line)
	{
		free_map(map_lines);
		close(fd);
		exit(EXIT_FAILURE);
	}
	cleaned = ft_trimend(new_line, " \t\r\n");
	free(new_line);
	if (!cleaned)
	{
		free_map(map_lines);
		close(fd);
		exit(EXIT_FAILURE);
	}
	return (cleaned);
}

static char	**process_map_lines(int fd, t_game *game, int capacity)
{
	char	**map_lines;
	char	*line;
	int		size;

	map_lines = init_map_lines(fd, game);
	size = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (size >= capacity - 1)
		{
			capacity *= 2;
			map_lines = resize_map_array(map_lines, line, capacity);
			if (!map_lines)
				return (NULL);
		}
		map_lines[size++] = process_line(line, map_lines, fd);
		line = get_next_line(fd);
	}
	map_lines[size] = NULL;
	return (map_lines);
}

char	**read_map_from_file(char *filename, t_game *game)
{
	int		fd;
	char	**map_lines;
	int		capacity;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		handle_file_error(fd, game);
	capacity = 100;
	map_lines = process_map_lines(fd, game, capacity);
	close(fd);
	return (map_lines);
}
