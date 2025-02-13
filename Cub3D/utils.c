#include "cub3d.h"

char *replace_tabs_with_spaces(char *line, int spaces_per_tab)
{
	int i = 0, j = 0;
	int len = ft_strlen(line);
	char *new_line = malloc(len * spaces_per_tab + 1);

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
	return new_line;
}

char **copy_map(char **map)
{
	int rows = 0;
	while (map[rows] != NULL) // Harita satırlarını say
		rows++;

	char **new_map = malloc((rows + 1) * sizeof(char *));
	if (!new_map)
	{
		perror("Error allocating memory for copy_map");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < rows; i++)
	{
		new_map[i] = ft_strdup(map[i]);
		if (!new_map[i]) // strdup başarısız olursa
		{
			perror("Error allocating memory for map row");
			while (i > 0)
				free(new_map[--i]); // Önceki tahsisleri temizle
			free(new_map);
			exit(EXIT_FAILURE);
		}
	}
	new_map[rows] = NULL; // Sonlandırıcı ekle
	return new_map;
}

void free_map(char **map)
{
	if (!map) // NULL kontrolü ekledik
		return;

	for (int i = 0; map[i] != NULL; i++)
		free(map[i]);

	free(map);
}
