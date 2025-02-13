#include "cub3d.h"

int key_press(int keycode, t_game *game)
{
    if (keycode == 65307)
        close_window(game);
    else if (keycode == 119)
        game->key->w = 1;
    else if (keycode == 115)
        game->key->s = 1;
    else if (keycode == 97)
        game->key->a = 1;
    else if (keycode == 100)
        game->key->d = 1;
    else if (keycode == 65363)
        game->key->left = 1;
    else if (keycode == 65361)
        game->key->right = 1;
    return (0);
}

int key_release(int keycode, t_game *game)
{
    if (keycode == 119)
        game->key->w = 0;
    else if (keycode == 115)
        game->key->s = 0;
    else if (keycode == 97)
        game->key->a = 0;
    else if (keycode == 100)
        game->key->d = 0;
    else if (keycode == 65363)
        game->key->left = 0;
    else if (keycode == 65361)
        game->key->right = 0;
    return (0);
}

int close_window(t_game *game)
{
    printf("Exiting game...\n");
    cleanup_all(game);
    exit(0);
}
