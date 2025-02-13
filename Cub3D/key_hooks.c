#include "cub3d.h"


void cleanup_game_n(t_game *game)
{
   if (game->player)
       free(game->player);
   if (game->raycast)
       free(game->raycast);
   if (game->key)
       free(game->key);
  
   // Duvar texture'larını temizle
   for (int i = 0; i < 4; i++)
   {
       if (game->walls[i])
       {
           if (game->walls[i]->img)
               mlx_destroy_image(game->mlx, game->walls[i]->img);
           free(game->walls[i]);
       }
   }
  
   // MLX kaynaklarını temizle
   if (game->img_ptr)
       mlx_destroy_image(game->mlx, game->img_ptr);
   if (game->win)
       mlx_destroy_window(game->mlx, game->win);
   if (game->mlx)
   {
       mlx_destroy_display(game->mlx);
       free(game->mlx);
   }
}
// Tuşa basıldığında
int key_press(int keycode, t_game *game)
{
    if (keycode == 65307) // ESC tuşu (Ubuntu)
    {
        printf("Oyundan çıkılıyor...\n");
        cleanup_all(game);
        //free(game);
        exit(0);
    }
    else if (keycode == 119) // W tuşu (Ubuntu)
        game->key->w = 1;
    else if (keycode == 115) // S tuşu (Ubuntu)
        game->key->s = 1;
    else if (keycode == 97)  // A tuşu (Ubuntu)
        game->key->a = 1;
    else if (keycode == 100) // D tuşu (Ubuntu)
        game->key->d = 1;
    else if (keycode == 65363) // Sol ok (Ubuntu)
        game->key->left = 1;
    else if (keycode == 65361) // Sağ ok (Ubuntu)
        game->key->right = 1;
    return (0);
}

// Tuş bırakıldığında
int key_release(int keycode, t_game *game)
{
    if (keycode == 119) // W tuşu
        game->key->w = 0;
    else if (keycode == 115) // S tuşu
        game->key->s = 0;
    else if (keycode == 97)  // A tuşuF
        game->key->a = 0;
    else if (keycode == 100) // D tuşu
        game->key->d = 0;
    else if (keycode == 65363) // Sol ok
        game->key->left = 0;
    else if (keycode == 65361) // Sağ ok
        game->key->right = 0;
    return (0);
}

// Pencere kapatıldığında
int close_window(t_game *game)
{
    printf("Oyundan çıkılıyor...\n");
    cleanup_all(game);
    exit(0);
    return (0);
}



