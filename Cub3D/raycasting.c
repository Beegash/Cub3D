#include "cub3d.h"
#include <math.h>

// Duvar yüksekliğini hesapla
void calculate_wall_height(t_game *game, t_raycast *ray)
{
    (void)game;  // Kullanılmayan parametre

    if (ray->side1 == 0)
        ray->perp_dist = (ray->sidedist_x - ray->deltadist_x);
    else
        ray->perp_dist = (ray->sidedist_y - ray->deltadist_y);

    // Duvar yüksekliğini hesapla (daha küçük bir ölçek faktörü kullanarak)
    ray->height = (int)(WINDOW_HEIGHT / (ray->perp_dist * 1.5));

    // Çizim başlangıç ve bitiş noktalarını hesapla
    ray->start_y = -ray->height / 2 + WINDOW_HEIGHT / 2;
    if (ray->start_y < 0)
        ray->start_y = 0;

    ray->end_y = ray->height / 2 + WINDOW_HEIGHT / 2;
    if (ray->end_y >= WINDOW_HEIGHT)
        ray->end_y = WINDOW_HEIGHT - 1;
}

// Işın yönünü ve adım değerlerini hesapla
void calculate_ray_dir(t_game *game, t_raycast *ray, int x)
{
    // Kamera düzlemindeki x koordinatı (-1 ile 1 arasında)
    ray->camera_x = 2 * x / (double)WINDOW_WIDTH - 1;

    // Işın yönü
    ray->raydir_x = game->player->dir_x + game->player->plane_x * ray->camera_x;
    ray->raydir_y = game->player->dir_y + game->player->plane_y * ray->camera_x;

    // DDA algoritması için delta mesafeleri
    ray->deltadist_x = fabs(1 / ray->raydir_x);
    ray->deltadist_y = fabs(1 / ray->raydir_y);

    // Başlangıç konumu (harita koordinatları)
    int map_x = (int)game->player->x;
    int map_y = (int)game->player->y;

    // Adım yönünü ve başlangıç mesafesini hesapla
    if (ray->raydir_x < 0)
    {
        ray->step_x = -1;
        ray->sidedist_x = (game->player->x - map_x) * ray->deltadist_x;
    }
    else
    {
        ray->step_x = 1;
        ray->sidedist_x = (map_x + 1.0 - game->player->x) * ray->deltadist_x;
    }
    if (ray->raydir_y < 0)
    {
        ray->step_y = -1;
        ray->sidedist_y = (game->player->y - map_y) * ray->deltadist_y;
    }
    else
    {
        ray->step_y = 1;
        ray->sidedist_y = (map_y + 1.0 - game->player->y) * ray->deltadist_y;
    }
}

// DDA algoritması ile duvar bul
void perform_dda(t_game *game, t_raycast *ray)
{
    int map_x = (int)game->player->x;
    int map_y = (int)game->player->y;
    ray->hit = 0;

    // DDA algoritması
    while (ray->hit == 0)
    {
        // En yakın kenarı seç
        if (ray->sidedist_x < ray->sidedist_y)
        {
            ray->sidedist_x += ray->deltadist_x;
            map_x += ray->step_x;
            ray->side1 = 0;
        }
        else
        {
            ray->sidedist_y += ray->deltadist_y;
            map_y += ray->step_y;
            ray->side1 = 1;
        }

        // Duvar kontrolü
        if (game->map->map_line[map_y][map_x] == '1')
            ray->hit = 1;
    }
}

// Duvar çizimi
void draw_wall(t_game *game, int x, t_raycast *ray)
{
    // Duvarın hangi texture'ı kullanacağını belirle
    int tex_num;
    if (ray->side1 == 0)
        tex_num = (ray->raydir_x > 0) ? 3 : 2;  // EA veya WE
    else
        tex_num = (ray->raydir_y > 0) ? 1 : 0;  // SO veya NO

    // Duvarın tam olarak nereye çarptığını hesapla
    double wall_x;
    if (ray->side1 == 0)
        wall_x = game->player->y + ray->perp_dist * ray->raydir_y;
    else
        wall_x = game->player->x + ray->perp_dist * ray->raydir_x;
    wall_x -= floor(wall_x);

    // Texture'daki x koordinatını hesapla
    ray->tex_x = (int)(wall_x * game->walls[tex_num]->w);
    if ((ray->side1 == 0 && ray->raydir_x > 0) || 
        (ray->side1 == 1 && ray->raydir_y < 0))
        ray->tex_x = game->walls[tex_num]->w - ray->tex_x - 1;

    // Her dikey çizgi için
    double step = 1.0 * game->walls[tex_num]->h / ray->height;
    double tex_pos = (ray->start_y - WINDOW_HEIGHT / 2 + ray->height / 2) * step;

    for (int y = ray->start_y; y < ray->end_y; y++)
    {
        ray->tex_y = (int)tex_pos & (game->walls[tex_num]->h - 1);
        tex_pos += step;

        // Texture'dan rengi al
        int color = game->walls[tex_num]->addr[game->walls[tex_num]->h * ray->tex_y + ray->tex_x];
        
        // Eğer yan duvarsa rengi biraz koyulaştır
        if (ray->side1 == 1)
            color = (color >> 1) & 8355711;

        // Rengi ekrana çiz
        if (x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT)
            game->mlx_data[y * WINDOW_WIDTH + x] = color;
    }
}

// Zemin ve tavan renklerini çiz
void draw_floor_ceiling(t_game *game)
{
    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            if (y < WINDOW_HEIGHT / 2)
            {
                // Tavan rengi
                int color = (game->map->ceiling_color[0] << 16) | 
                           (game->map->ceiling_color[1] << 8) | 
                            game->map->ceiling_color[2];
                game->mlx_data[y * WINDOW_WIDTH + x] = color;
            }
            else
            {
                // Zemin rengi
                int color = (game->map->floor_color[0] << 16) | 
                           (game->map->floor_color[1] << 8) | 
                            game->map->floor_color[2];
                game->mlx_data[y * WINDOW_WIDTH + x] = color;
            }
        }
    }
}

// Ana raycasting fonksiyonu
void perform_raycasting(t_game *game)
{
    // Önce zemin ve tavanı çiz
    draw_floor_ceiling(game);

    // Her ekran sütunu için ray-casting yap
    for (int x = 0; x < WINDOW_WIDTH; x++)
    {
        // Ray yönünü hesapla
        calculate_ray_dir(game, game->raycast, x);
        
        // DDA algoritması ile duvar kesişimini bul
        perform_dda(game, game->raycast);
        
        // Duvar yüksekliğini hesapla
        calculate_wall_height(game, game->raycast);
        
        // Duvarı çiz
        draw_wall(game, x, game->raycast);
    }
} 