/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_texture_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:40:38 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/14 03:43:11 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	check_single_value(char *number, char **numbers)
{
	char	*trimmed;

	trimmed = ft_strtrim(number);
	if (!trimmed || !*trimmed)
	{
		free(trimmed);
		free_map(numbers);
		return (0);
	}
	if (!is_valid_number(trimmed))
	{
		free(trimmed);
		free_map(numbers);
		return (0);
	}
	free(trimmed);
	return (1);
}

static int	validate_rgb_values(char **numbers)
{
	int		i;

	i = 0;
	while (numbers[i])
	{
		if (!check_single_value(numbers[i], numbers))
			return (0);
		i++;
	}
	if (i != 3)
	{
		free_map(numbers);
		return (0);
	}
	return (1);
}

static int	convert_rgb_values(char **numbers, int *rgb)
{
	int		i;
	char	*trimmed;

	i = 0;
	while (i < 3)
	{
		trimmed = ft_strtrim(numbers[i]);
		if (!trimmed)
		{
			free_map(numbers);
			return (0);
		}
		rgb[i] = ft_atoi(trimmed);
		free(trimmed);
		if (rgb[i] < 0 || rgb[i] > 255)
		{
			free_map(numbers);
			return (0);
		}
		i++;
	}
	return (1);
}

int	rgb_numbers(char *line, int j, int *rgb)
{
	char	**numbers;

	while (line[j] && (line[j] == '\t' || line[j] == ' '))
		j++;
	if (ft_strcount(line + j, ',') > 2)
		return (0);
	numbers = ft_split(line + j, ',');
	if (!numbers)
		return (0);
	if (!validate_rgb_values(numbers))
		return (0);
	if (!convert_rgb_values(numbers, rgb))
		return (0);
	free_map(numbers);
	return (1);
}
