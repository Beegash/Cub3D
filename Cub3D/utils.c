/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iozmen <iozmen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:45:24 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/13 20:45:25 by iozmen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*replace_tabs_with_spaces(char *line, int spaces_per_tab)
{
	int		i = 0, j;
	int		len;
	char	*new_line;

	i = 0, j = 0;
	len = ft_strlen(line);
	new_line = malloc(len * spaces_per_tab + 1);
	while (line[i] != '\0')
	{
		if (line[i] == '\t')
		{
			for (int k = 0; k < spaces_per_tab; k++)
			{
				new_line[j++] = ' ';
			}
		}
		else
		{
			new_line[j++] = line[i];
		}
		i++;
	}
	new_line[j] = '\0';
	return (new_line);
}

char	**copy_map(char **map)
{
	int		rows;
	char	**new_map;

	rows = 0;
	while (map[rows] != NULL)
		rows++;
	new_map = malloc((rows + 1) * sizeof(char *));
	if (!new_map)
	{
		perror("Error allocating memory for copy_map");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < rows; i++)
	{
		new_map[i] = ft_strdup(map[i]);
		if (!new_map[i])
		{
			perror("Error allocating memory for map row");
			while (i > 0)
				free(new_map[--i]);
			free(new_map);
			exit(EXIT_FAILURE);
		}
	}
	new_map[rows] = NULL;
	return (new_map);
}

void	free_map(char **map)
{
	if (!map)
		return ;
	for (int i = 0; map[i] != NULL; i++)
		free(map[i]);
	free(map);
}
