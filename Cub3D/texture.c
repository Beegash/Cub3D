/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iozmen <iozmen@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:47:05 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/13 20:47:19 by iozmen           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./get_next_line/get_next_line.h"
#include "cub3d.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	res;

	i = 0;
	neg = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - '0') + (res * 10);
		i++;
	}
	return (res * neg);
}
int	ft_trimplus(char *str, int start, int end)
{
	while (start < end)
	{
		if (str[start] == ' ' || str[start] == '\t')
		{
			printf("Error Incorrect Amount Of Number : \"%c\" \n", str[start]);
			return (0);
		}
		start++;
	}
	return (1);
}
char	*ft_strtrim(char *str)
{
	int		start;
	int		end;
	char	*trimmed;
	int		i;

	start = 0;
	while (str[start] && (str[start] == ' ' || str[start] == '\t'))
		start++;
	end = ft_strlen(str) - 1;
	while (end > start && (str[end] == ' ' || str[end] == '\t'
			|| str[end] == '\n'))
		end--;
	if (ft_trimplus(str, start, end) == 0)
		return (NULL);
	trimmed = (char *)malloc(sizeof(char) * (end - start + 2));
	if (!trimmed)
		return (NULL);
	i = 0;
	while (start <= end)
		trimmed[i++] = str[start++];
	trimmed[i] = '\0';
	return (trimmed);
}

char	*texture_path(char *line, int j)
{
	char	*full_line;
	char	*trimmed;

	while (line[j] && (line[j] == '\t' || line[j] == ' '))
		j++;
	full_line = ft_strdup(line + j);
	if (!full_line)
		return (NULL);
	trimmed = ft_strtrim(full_line);
	free(full_line);
	if (!trimmed)
		return (NULL);
	return (trimmed);
}

static int	handle_texture_identifier(char *line, int j, t_map *map)
{
	if (line[j] == 'N' && line[j + 1] == 'O' && !(map->north_texture))
		map->north_texture = texture_path(line, j + 2);
	else if (line[j] == 'S' && line[j + 1] == 'O' && !(map->south_texture))
		map->south_texture = texture_path(line, j + 2);
	else if (line[j] == 'W' && line[j + 1] == 'E' && !(map->west_texture))
		map->west_texture = texture_path(line, j + 2);
	else if (line[j] == 'E' && line[j + 1] == 'A' && !(map->east_texture))
		map->east_texture = texture_path(line, j + 2);
	return (1);
}

static int	handle_color_identifier(char *line, int j, t_map *map)
{
	if (line[j] == 'F' && !(map->floor_color[0]))
	{
		j++;
		if (!rgb_numbers(line, j, map->floor_color))
		{
			printf("Error in F color format\n");
			return (0);
		}
	}
	else if (line[j] == 'C' && !(map->ceiling_color[0]))
	{
		j++;
		if (!rgb_numbers(line, j, map->ceiling_color))
		{
			printf("Error in C color format\n");
			return (0);
		}
	}
	return (1);
}

static int	process_map_line(char *line, int j, t_map *map)
{
	while (line[j] && (line[j] == '\t' || line[j] == ' ' || line[j] == '\n'))
		j++;
	if (!line[j])
		return (-1);
	if (line[j] == 'N' || line[j] == 'S' || line[j] == 'W' || line[j] == 'E')
		return (handle_texture_identifier(line, j, map));
	else if (line[j] == 'F' || line[j] == 'C')
		return (handle_color_identifier(line, j, map));
	else if (line[j] == '1' || line[j] == '0')
		return (0);
	else if (line[j] != '\0' && line[j] != '\n')
	{
		printf("Error: Invalid identifier: [%c] (ASCII: %d)\n", line[j],
			line[j]);
		return (-2);
	}
	return (1);
}

int	get_texture(char **map_line, t_map *map)
{
	int	i;
	int	result;

	i = 0;
	while (map_line[i])
	{
		result = process_map_line(map_line[i], 0, map);
		if (result == 0)
			return (i);
		if (result == -2)
			return (0);
		if (result == -1)
		{
			i++;
			continue ;
		}
		i++;
	}
	return (i);
}

static int	validate_rgb_values(char **numbers)
{
	int		i;
	char	*trimmed;

	i = 0;
	while (numbers[i])
	{
		trimmed = ft_strtrim(numbers[i]);
		if (!trimmed || !*trimmed)
		{
			free(trimmed);
			free_map(numbers);
			return (error_message("Empty color value found", 0));
		}
		if (!is_valid_number(trimmed))
		{
			free(trimmed);
			free_map(numbers);
			return (error_message("Invalid number format in color value", 0));
		}
		free(trimmed);
		i++;
	}
	if (i != 3)
	{
		free_map(numbers);
		return (error_message("Color must have exactly 3 values (R,G,B)", 0));
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
			return (error_message("Memory allocation failed for trimming", 0));
		}
		rgb[i] = ft_atoi(trimmed);
		free(trimmed);
		if (rgb[i] < 0 || rgb[i] > 255)
		{
			free_map(numbers);
			return (error_message("Color values must be between 0 and 255", 0));
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
	numbers = split(line + j, ',');
	if (!numbers)
		return (error_message("Memory allocation failed for color values", 0));
	if (!validate_rgb_values(numbers))
		return (0);
	if (!convert_rgb_values(numbers, rgb))
		return (0);
	free_map(numbers);
	return (1);
}

int	is_valid_number(char *str)
{
	int i = 0;

	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}