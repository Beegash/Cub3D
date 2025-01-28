#include "cub3d.h"
// DURSUN NOLUR NOLMAZ
#define INITIAL_BUFFER_SIZE 256

void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    if (new_size == 0)
    {
        free(ptr);
        return NULL;
    }

    void *new_ptr = malloc(new_size);
    if (!new_ptr)
    {
        perror("Error allocating memory in ft_realloc");
        exit(EXIT_FAILURE);
    }

    if (ptr)
    {
        size_t copy_size = old_size < new_size ? old_size : new_size;
        ft_memcpy(new_ptr, ptr, copy_size);
        free(ptr);
    }
    return new_ptr;
}

char **read_map_from_file(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char **map_lines = NULL;
    size_t line_count = 0;
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    char *buffer = malloc(buffer_size);
    if (!buffer)
    {
        perror("Error allocating initial buffer");
        close(fd);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read;
    size_t buffer_index = 0;

    while ((bytes_read = read(fd, buffer + buffer_index, 1)) > 0)
    {
        if (buffer[buffer_index] == '\n')
        {
            buffer[buffer_index] = '\0';
            map_lines = ft_realloc(map_lines, line_count * sizeof(char *), (line_count + 1) * sizeof(char *));

            map_lines[line_count] = malloc(buffer_index + 1);
            if (!map_lines[line_count])
            {
                perror("Error allocating memory for map line");
                close(fd);
                exit(EXIT_FAILURE);
            }
            ft_memcpy(map_lines[line_count], buffer, buffer_index + 1);
            line_count++;
            buffer_index = 0;
        }
        else
        {
            buffer_index++;

            if (buffer_index >= buffer_size)
            {
                buffer_size *= 2;
                buffer = ft_realloc(buffer, buffer_size / 2, buffer_size);
            }
        }
    }

    if (bytes_read == -1)
    {
        perror("Error reading file");
        close(fd);
        free(buffer);
        exit(EXIT_FAILURE);
    }

    if (buffer_index > 0)
    {
        buffer[buffer_index] = '\0';
        map_lines = ft_realloc(map_lines, line_count * sizeof(char *), (line_count + 1) * sizeof(char *));
        map_lines[line_count] = malloc(buffer_index + 1);
        if (!map_lines[line_count])
        {
            perror("Error allocating memory for final map line");
            close(fd);
            free(buffer);
            exit(EXIT_FAILURE);
        }
        ft_memcpy(map_lines[line_count], buffer, buffer_index + 1);
        line_count++;
    }

    map_lines = ft_realloc(map_lines, line_count * sizeof(char *), (line_count + 1) * sizeof(char *));
    map_lines[line_count] = NULL;

    free(buffer);
    close(fd);
    return map_lines;
}

void free_map(char **map)
{
    for (int i = 0; map[i] != NULL; i++)
    {
        free(map[i]);
    }
    free(map);
}
