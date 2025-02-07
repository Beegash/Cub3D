#include "cub3d.h"

int init_game(t_game *game)
{
    if (!game)
        return (0);
    
    // MLX başlatma
    game->mlx = mlx_init();
    if (!game->mlx)
        return (0);
    
    // Pencere oluşturma
    game->win = mlx_new_window(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT, "Cub3D");
    if (!game->win)
        return (0);
    
    // Render için görüntü oluşturma
    game->img_ptr = mlx_new_image(game->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!game->img_ptr)
        return (0);
    
    // Görüntü verilerine erişim
    game->mlx_data = (int *)mlx_get_data_addr(game->img_ptr, &game->pixel_bits,
                                      &game->line_bytes, &game->endian);
    
    // Olay işleyicilerini ayarlama
    mlx_hook(game->win, 2, 1L<<0, key_press, game);    // Tuş basma
    mlx_hook(game->win, 3, 1L<<1, key_release, game);  // Tuş bırakma
    mlx_hook(game->win, 17, 0, exit_game, game);       // Pencere kapatma
    
    return (1);
}

int game_loop(t_game *game)
{
    // Oyun mantığı ve render işlemleri burada gerçekleştirilecek
    
    // Görüntüyü pencereye çizme
    mlx_put_image_to_window(game->mlx, game->win, game->img_ptr, 0, 0);
    
    return (0);
}

int key_press(int keycode, t_game *game)
{
    if (keycode == 53)  // ESC tuşu
        exit_game(game);
    // Diğer tuş işlemleri burada eklenecek
    return (0);
}

int key_release(int keycode, t_game *game)
{
    (void)keycode;
    (void)game;
    // Tuş bırakma işlemleri burada eklenecek
    return (0);
}

int exit_game(t_game *game)
{
    // Kaynakları temizleme
    if (game->img_ptr)
        mlx_destroy_image(game->mlx, game->img_ptr);
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    // Diğer temizleme işlemleri...
    
    exit(0);
    return (0);
} 