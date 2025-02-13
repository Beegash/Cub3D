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
   if (!game || !game->mlx)
       return;


   // Cleanup wall textures
   for (int i = 0; i < 4; i++)
   {
       if (game->walls[i])
       {
           if (game->walls[i]->img)
               mlx_destroy_image(game->mlx, game->walls[i]->img);
           free(game->walls[i]);
       }
   }


   // Cleanup main image
   if (game->img_ptr)
       mlx_destroy_image(game->mlx, game->img_ptr);


   // Cleanup window and mlx
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


   // Cleanup map structure
   cleanup_map_content(game->map);
   if (game->map)
       free(game->map);


   // Cleanup MLX related content
   cleanup_mlx_content(game);


   // Cleanup other structures
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


   // Finally, free the game structure itself
   free(game);
}
