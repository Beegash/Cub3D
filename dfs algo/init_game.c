#include "cub3d.h"

// Oyun yapısını başlat
static void init_game_struct(t_game *game)
{
    game->mlx = NULL;
    game->win = NULL;
    game->img_ptr = NULL;
    game->mlx_data = NULL;
    
    // Duvar texture'larını NULL'a ayarla
    for (int i = 0; i < 4; i++)
        game->walls[i] = NULL;
    
    // Oyuncu, raycast ve tuş yapılarını oluştur
    game->player = malloc(sizeof(t_player));
    game->raycast = malloc(sizeof(t_raycast));
    game->key = malloc(sizeof(t_keycode));
    
    if (!game->player || !game->raycast || !game->key)
    {
        printf("Hata: Bellek ayırma başarısız\n");
        exit(1);
    }

    // Oyuncu başlangıç pozisyonunu ayarla
    game->player->x = (double)game->loc_px + 0.5;  // Karenin ortasına yerleştir
    game->player->y = (double)game->loc_py + 0.5;

    // Oyuncu yönünü ayarla (başlangıçta karakterin baktığı yöne göre)
    if (game->playertype == 'N')
    {
        game->player->dir_x = 0;
        game->player->dir_y = -1;
        game->player->plane_x = 0.66;
        game->player->plane_y = 0;
    }
    else if (game->playertype == 'S')
    {
        game->player->dir_x = 0;
        game->player->dir_y = 1;
        game->player->plane_x = -0.66;
        game->player->plane_y = 0;
    }
    else if (game->playertype == 'E')
    {
        game->player->dir_x = 1;
        game->player->dir_y = 0;
        game->player->plane_x = 0;
        game->player->plane_y = 0.66;
    }
    else if (game->playertype == 'W')
    {
        game->player->dir_x = -1;
        game->player->dir_y = 0;
        game->player->plane_x = 0;
        game->player->plane_y = -0.66;
    }

    // Raycast yapısını sıfırla
    ft_memset(game->raycast, 0, sizeof(t_raycast));
    
    // Tuş durumlarını sıfırla
    game->key->w = 0;
    game->key->a = 0;
    game->key->s = 0;
    game->key->d = 0;
    game->key->left = 0;
    game->key->right = 0;
}

// MLX'i başlat ve pencere oluştur
static int init_mlx(t_game *game)
{
    game->mlx = mlx_init();
    if (!game->mlx)
        return (0);

    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
    if (!game->win)
        return (0);

    // Ekran image'ini oluştur
    game->img_ptr = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!game->img_ptr)
        return (0);

    // Pixel verilerine erişim için
    game->mlx_data = (int *)mlx_get_data_addr(game->img_ptr, &game->pixel_bits,
                                             &game->line_bytes, &game->endian);
    return (1);
}

// Texture'ları yükle
static int load_textures(t_game *game)
{
    char *texture_paths[4] = {
        game->map->north_texture,    // Kuzey duvarı
        game->map->south_texture,    // Güney duvarı
        game->map->west_texture,     // Batı duvarı
        game->map->east_texture      // Doğu duvarı
    };

    for (int i = 0; i < 4; i++)
    {
        game->walls[i] = malloc(sizeof(t_textures));
        if (!game->walls[i])
            return (0);

        game->walls[i]->img = mlx_xpm_file_to_image(game->mlx, texture_paths[i],
                                                   &game->walls[i]->w, &game->walls[i]->h);
        if (!game->walls[i]->img)
            return (0);

        game->walls[i]->addr = (int *)mlx_get_data_addr(game->walls[i]->img,
                                                       &game->walls[i]->bpp,
                                                       &game->walls[i]->line_len,
                                                       &game->walls[i]->endian);
    }
    return (1);
}

// Oyunu başlat
int init_game(t_game *game)
{
    init_game_struct(game);
    
    if (!init_mlx(game))
        return (0);
    
    if (!load_textures(game))
        return (0);
    
    return (1);
} 