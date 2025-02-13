#include "cub3d.h"

void cleanup_map_content(t_map *map)
{
   if (!map)
       return;
   if (map->map_line)
       free_map(map->map_line);
   if (map->north_texture)
       free(map->north_texture);
   if (map->south_texture)
       free(map->south_texture);
   if (map->west_texture)
       free(map->west_texture);
   if (map->east_texture)
       free(map->east_texture);
}

void cleanup_mlx_content(t_game *game)
{
    int i;

    i = 0;
   //if (!game || !game->mlx)
       //return;
   while (i < 4)
   {
       if (game->walls[i])
       {
           if (game->walls[i]->img)
               mlx_destroy_image(game->mlx, game->walls[i]->img);
           free(game->walls[i]);
       }
       i++;
   }
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

void cleanup_all(t_game *game)
{
   if (!game)
       return;
   cleanup_map_content(game->map);
   if (game->map)
       free(game->map);
   cleanup_mlx_content(game);
   if (game->img)
       free(game->img);
   if (game->key)
       free(game->key);
   if (game->player)
       free(game->player);
   if (game->raycast)
       free(game->raycast);
   if (game->mapname)
       free(game->mapname);
   free(game);
}
