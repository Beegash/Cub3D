#include "cub3d.h"
#include "get_next_line.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	neg;
	int	res;

	i = 0;
	neg = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			neg *= -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (str[i] - '0') + (res * 10);
		i++;
	}
	return (res * neg);
}
int ft_trimplus(char *str, int start, int end)
{
    while(start < end)
    {
        if(str[start] == ' ' || str[start] == '\t')
        {
            printf("Error Incorrect Amount Of Number : \"%c\" \n",str[start]);
            return(0);
        }
        start++;
    }
    return(1);
}
char *ft_strtrim(char *str)
{
    int start;
    int end;
    char *trimmed;
    int i;

    start = 0;
    while (str[start] && (str[start] == ' ' || str[start] == '\t'))
        start++;
    end = ft_strlen(str) - 1;
    while (end > start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n'))
        end--;
    if(ft_trimplus(str,start,end) == 0)
        return (NULL);
    trimmed = (char *)malloc(sizeof(char) * (end - start + 2));
    if (!trimmed)
        return NULL;
    i = 0;
    while (start <= end)
        trimmed[i++] = str[start++];
    trimmed[i] = '\0';
    return trimmed;
}

char *texture_path(char *line, int j)
{
    char *full_line;
    char *trimmed;

    // Baştaki boşlukları atla
    while(line[j] && (line[j] == '\t' || line[j] == ' '))
        j++;
    
    // Texture yolunu al ve boşlukları temizle
    full_line = ft_strdup(line + j);
    if (!full_line)
        return NULL;

    trimmed = ft_strtrim(full_line);
    free(full_line);
    if (!trimmed)
        return NULL;

    return trimmed;
}

int get_texture(char **map_line, t_map *map)
{
    int i = 0;
    int j;

    while (map_line[i])
    {
        j = 0;
        while(map_line[i][j] && map_line[i][j] == '\n')
            i++;
        while(map_line[i][j] && (map_line[i][j] == '\t' || map_line[i][j] == ' '))
            j++;

        if (map_line[i][j] == '1' || map_line[i][j] == '0')
            return i;

        if(map_line[i][j] == 'N' && map_line[i][j+1] == 'O' && !(map->north_texture))
            map->north_texture = texture_path(map_line[i], j+2);
        else if(map_line[i][j] == 'S' && map_line[i][j+1] == 'O' && !(map->south_texture))
            map->south_texture = texture_path(map_line[i], j+2);
        else if(map_line[i][j] == 'W' && map_line[i][j+1] == 'E' && !(map->west_texture))
            map->west_texture = texture_path(map_line[i], j+2);
        else if(map_line[i][j] == 'E' && map_line[i][j+1] == 'A' && !(map->east_texture))
            map->east_texture = texture_path(map_line[i], j+2);
        else if(map_line[i][j] == 'C' && !(map->ceiling_color[0]))
        {
            if (!rgb_numbers(map_line[i], j+1, map->ceiling_color))
                return 0;
        }
        else if(map_line[i][j] == 'F' && !(map->floor_color[0]))
        {
            if (!rgb_numbers(map_line[i], j+1, map->floor_color))
                return 0;
        }
        else
        {
            printf("Error: Invalid identifier at line %d: %c\n", i+1, map_line[i][j]);
            return 0;
        }
        i++;
    }
    return 1;
}

int rgb_numbers(char *line, int j, int *rgb)
{
    char **numbers;
    char *trimmed;
    int i;

    while(line[j] && (line[j] == '\t' || line[j] == ' '))
        j++;
    
    numbers = split(line + j, ',');
    if (!numbers)
    {
        printf("Error: Memory allocation failed for color values\n");
        return 0;
    }

    i = 0;
    while (numbers[i]){

        printf("%s",numbers[i]);
        i++;
    }
    if (i != 3)
    {
        printf("Error: Color must have exactly 3 values (R,G,B)\n");
        free_map(numbers);
        return 0;
    }

    i = 0;
    while (i < 3)
    {
        trimmed = ft_strtrim(numbers[i]);
        if (!trimmed)
        {
            printf("Error: Memory allocation failed for trimming\n");
            free_map(numbers);
            return 0;
        }
        rgb[i] = ft_atoi(trimmed);
        free(trimmed);
        if (rgb[i] < 0 || rgb[i] > 255)
        {
            printf("Error: Color values must be between 0 and 255\n");
            free_map(numbers);
            return 0;
        }
        i++;
    }
    free_map(numbers);
    return 1;
}

int main(void)
{
    t_map map;
    char **map_lines;

    // Map yapısını sıfırla
    map.north_texture = NULL;
    map.south_texture = NULL;
    map.west_texture = NULL;
    map.east_texture = NULL;
    for(int i = 0; i < 3; i++)
    {
        map.floor_color[i] = 0;
        map.ceiling_color[i] = 0;
    }

    // Map dosyasını oku
    map_lines = read_map_from_file("map.cub");
    if (!map_lines)
    {
        printf("Hata: map.cub dosyası okunamadı\n");
        return 1;
    }

    // Texture ve renkleri işle
    int result = get_texture(map_lines, &map);
    if (result == 0)
    {
        printf("Hata: Texture veya renk değerleri geçersiz\n");
        free_map(map_lines);
        return 1;
    }

    // Sonuçları yazdır
    printf("Texture paths:\n");
    printf("North: %s\n", map.north_texture ? map.north_texture : "Not set");
    printf("South: %s\n", map.south_texture ? map.south_texture : "Not set");
    printf("West: %s\n", map.west_texture ? map.west_texture : "Not set");
    printf("East: %s\n", map.east_texture ? map.east_texture : "Not set");

    printf("\nColors:\n");
    printf("Floor: RGB(%d,%d,%d)\n", map.floor_color[0], map.floor_color[1], map.floor_color[2]);
    printf("Ceiling: RGB(%d,%d,%d)\n", map.ceiling_color[0], map.ceiling_color[1], map.ceiling_color[2]);

    // Belleği temizle
    free_map(map_lines);
    free(map.north_texture);
    free(map.south_texture);
    free(map.west_texture);
    free(map.east_texture);

    return 0;
}