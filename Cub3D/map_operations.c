/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iozmen <iozmen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:45:24 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/14 16:50:29 by iozmen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	add_spaces(char *new_line, int *j, int spaces_per_tab)
{
	int	k;

	k = 0;
	while (k < spaces_per_tab)
	{
		new_line[*j] = ' ';
		(*j)++;
		k++;
	}
}

char	*replace_tabs_with_spaces(char *line, int spaces_per_tab)
{
	int		i;
	int		j;
	int		len;
	char	*new_line;

	i = 0;
	j = 0;
	len = ft_strlen(line);
	new_line = malloc(len * spaces_per_tab + 1);
	while (line[i] != '\0')
	{
		if (line[i] == '\t')
			add_spaces(new_line, &j, spaces_per_tab);
		else
		{
			new_line[j] = line[i];
			j++;
		}
		i++;
	}
	new_line[j] = '\0';
	return (new_line);
}

char	**copy_map(char **map)
{
	int		rows;
	int		i;
	char	**new_map;

	rows = 0;
	while (map[rows] != NULL)
		rows++;
	new_map = malloc((rows + 1) * sizeof(char *));
	if (!new_map)
		handle_malloc_error(new_map, 0, "Error allocating memory for copy_map");
	i = 0;
	while (i < rows)
	{
		new_map[i] = ft_strdup(map[i]);
		if (!new_map[i])
			handle_malloc_error(new_map, i,
				"Error allocating memory for map row");
		i++;
	}
	new_map[rows] = NULL;
	return (new_map);
}

void	free_map(char **map)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (map[i] != NULL)
	{
		free(map[i]);
		i++;
	}
	free(map);
}
