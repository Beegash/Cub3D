/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:08:00 by ifozmen           #+#    #+#             */
/*   Updated: 2025/02/14 03:08:48 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	ft_strlen(const char *c)
{
	int	i;

	i = 0;
	if (!c)
		return (0);
	while (c[i] != '\0')
		i++;
	return (i);
}

int	ft_find(char *finder)
{
	int	i;

	i = 0;
	if (!finder)
		return (1);
	while (finder && finder[i])
	{
		if (finder[i] == '\n')
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strjoin(char *lft_str, char *buff)
{
	int		i;
	int		j;
	char	*str;

	if (!lft_str)
	{
		lft_str = malloc (sizeof(char) * 1);
		lft_str[0] = '\0';
	}
	if (!buff)
		return (0);
	str = malloc (sizeof(char) * (ft_strlen(lft_str) + ft_strlen(buff) + 1));
	if (!str)
		return (0);
	i = -1;
	while (lft_str[++i] != '\0')
		str[i] = lft_str[i];
	j = 0;
	while (buff[j] != '\0')
		str[i++] = buff[j++];
	str[i] = '\0';
	free(lft_str);
	return (str);
}

char	*ft_line(char *handle_string)
{
	char	*ptr;
	int		i;

	i = 0;
	if (!handle_string[i])
	{
		return (NULL);
	}
	while (handle_string[i] && handle_string[i] != '\n')
		i++;
	ptr = malloc (sizeof(char) * (i + 2));
	i = 0;
	if (!ptr)
		return (0);
	while (handle_string[i] && handle_string[i] != '\n')
	{
		ptr[i] = handle_string[i];
		i++;
	}
	if (handle_string[i] == '\n')
		ptr[i++] = '\n';
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_clean(char *handle_string)
{
	int		i;
	int		j;
	char	*ptr;

	i = 0;
	j = 0;
	while (handle_string[i] && handle_string[i] != '\n')
		i++;
	if (!handle_string[i])
	{
		free(handle_string);
		return (0);
	}
	ptr = malloc (sizeof(char) * (ft_strlen(handle_string) - i + 1));
	i++;
	while (handle_string[i])
	{
		ptr[j] = handle_string[i];
		i++;
		j++;
	}
	ptr[j] = '\0';
	free(handle_string);
	return (ptr);
}
