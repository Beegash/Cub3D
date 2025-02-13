#include "cub3d.h"

static void set_player_direction(t_game *game, double dir_x, double dir_y,
                               double plane_x, double plane_y)
{
    game->player->dir_x = dir_x;
    game->player->dir_y = dir_y;
    game->player->plane_x = plane_x;
    game->player->plane_y = plane_y;
}

static void init_player_direction(t_game *game)
{
    if (game->playertype == 'N')
        set_player_direction(game, 0, -1, 0.66, 0);
    else if (game->playertype == 'S')
        set_player_direction(game, 0, 1, -0.66, 0);
    else if (game->playertype == 'E')
        set_player_direction(game, 1, 0, 0, 0.66);
    else if (game->playertype == 'W')
        set_player_direction(game, -1, 0, 0, -0.66);
}

static void init_key_states(t_game *game)
{
    game->key->w = 0;
    game->key->a = 0;
    game->key->s = 0;
    game->key->d = 0;
    game->key->left = 0;
    game->key->right = 0;
}

static void init_basic_components(t_game *game)
{
    int i;

    i = 0;
    game->mlx = NULL;
    game->win = NULL;
    game->img_ptr = NULL;
    game->mlx_data = NULL;
    while (i < 4)
    {
        game->walls[i] = NULL;
        i++;
    }
    game->player = malloc(sizeof(t_player));
    game->raycast = malloc(sizeof(t_raycast));
    game->key = malloc(sizeof(t_keycode));
    if (!game->player || !game->raycast || !game->key)
        exit(error_message("Memory allocation failed", 1));
    game->player->x = (double)game->loc_px + 0.5;
    game->player->y = (double)game->loc_py + 0.5;
    ft_memset(game->raycast, 0, sizeof(t_raycast));
}

static void init_game_struct(t_game *game)
{
    init_basic_components(game);
    init_player_direction(game);
    init_key_states(game);
}

static int init_single_texture(t_game *game, int i, char *texture_path)
{
    game->walls[i] = malloc(sizeof(t_textures));
    if (!game->walls[i])
        return (error_message("Memory allocation failed", 0));
    game->walls[i]->img = mlx_xpm_file_to_image(game->mlx, texture_path,
                                               &game->walls[i]->w, &game->walls[i]->h);
    if (!game->walls[i]->img)
        return (error_message("Texture loading failed", 0));
    game->walls[i]->addr = (int *)mlx_get_data_addr(game->walls[i]->img,
                                                   &game->walls[i]->bpp,
                                                   &game->walls[i]->line_len,
                                                   &game->walls[i]->endian);
    return (1);
}

static int load_textures(t_game *game)
{
    int i;
    char *texture_paths[4] = {
        game->map->north_texture,
        game->map->south_texture,
        game->map->west_texture,
        game->map->east_texture
    };

    i = 0;
    while (i < 4)
    {
        if (!init_single_texture(game, i, texture_paths[i]))
            return (0);
        i++;
    }
    return (1);
}

static int init_mlx(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
        return (error_message("MLX initialization failed", 0));
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
    if (!game->win)
        return (error_message("Window creation failed", 0));
    game->img_ptr = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!game->img_ptr)
        return (error_message("Image creation failed", 0));
    game->mlx_data = (int *)mlx_get_data_addr(game->img_ptr, &game->pixel_bits,
                                             &game->line_bytes, &game->endian);
    return (1);
}

int init_game(t_game *game)
{
    init_game_struct(game);
    
    if (!init_mlx(game))
        return (0);
    
    if (!load_textures(game))
        return (0);
    
    return (1);
} 