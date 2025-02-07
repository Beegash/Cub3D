#include "cub3d.h"

// Her kare için çağrılacak fonksiyon
int game_loop(t_game *game)
{
    // Ekranı temizle
    ft_memset(game->mlx_data, 0, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(int));

    // Burada daha sonra ray-casting yapılacak

    // Ekranı güncelle
    mlx_put_image_to_window(game->mlx, game->win, game->img_ptr, 0, 0);
    return (0);
}

// Texture yollarını kontrol et
int check_textures(t_map *map)
{
    if (!map->north_texture || !map->south_texture || !map->west_texture || !map->east_texture)
    {
        printf("Hata: Eksik texture yolu\n");
        return (0);
    }

    // Texture dosyalarının varlığını kontrol et
    int fd;
    
    fd = open(map->north_texture, O_RDONLY);
    if (fd < 0)
    {
        printf("Hata: North texture dosyası bulunamadı: %s\n", map->north_texture);
        return (0);
    }
    close(fd);

    fd = open(map->south_texture, O_RDONLY);
    if (fd < 0)
    {
        printf("Hata: South texture dosyası bulunamadı: %s\n", map->south_texture);
        return (0);
    }
    close(fd);

    fd = open(map->west_texture, O_RDONLY);
    if (fd < 0)
    {
        printf("Hata: West texture dosyası bulunamadı: %s\n", map->west_texture);
        return (0);
    }
    close(fd);

    fd = open(map->east_texture, O_RDONLY);
    if (fd < 0)
    {
        printf("Hata: East texture dosyası bulunamadı: %s\n", map->east_texture);
        return (0);
    }
    close(fd);

    return (1);
}

int main(void)
{
    t_game game;
    char **map_lines;
    char **actual_map;

    // Oyun yapısını sıfırlama
    ft_memset(&game, 0, sizeof(t_game));
    
    // Map yapısını oluştur ve başlat
    game.map = malloc(sizeof(t_map));
    if (!game.map)
    {
        printf("Hata: Map yapısı için bellek ayrılamadı\n");
        return (1);
    }
    ft_memset(game.map, 0, sizeof(t_map));
    
    // Map dosyasını oku
    map_lines = read_map_from_file("map.cub");
    if (!map_lines)
    {
        printf("Hata: map.cub dosyası okunamadı\n");
        free(game.map);
        return (1);
    }

    // Texture ve renkleri işle
    int map_start = get_texture(map_lines, game.map);
    if (map_start == 0)
    {
        printf("Hata: Texture veya renk değerleri geçersiz\n");
        free(game.map);
        free_map(map_lines);
        return (1);
    }

    // Texture yollarını kontrol et
    if (!check_textures(game.map))
    {
        free(game.map);
        free_map(map_lines);
        return (1);
    }

    // Harita kısmını ayır
    int i = 0;
    while (map_lines[map_start + i])
        i++;
    actual_map = malloc(sizeof(char *) * (i + 1));
    if (!actual_map)
    {
        printf("Hata: Harita için bellek ayrılamadı\n");
        free(game.map);
        free_map(map_lines);
        return (1);
    }
    i = 0;
    while (map_lines[map_start + i])
    {
        actual_map[i] = ft_strdup(map_lines[map_start + i]);
        i++;
    }
    actual_map[i] = NULL;

    // Map verilerini yapıya ata
    game.map->map_line = actual_map;
    validate_map(game.map);
    
    // Oyunu başlat
    if (!init_game(&game))
    {
        printf("Oyun başlatılamadı! MLX başlatma hatası.\n");
        free(game.map);
        free_map(map_lines);
        free_map(actual_map);
        return (1);
    }

    // Hook'ları ayarla
    mlx_hook(game.win, 2, 1L<<0, key_press, &game);       // Tuşa basma
    mlx_hook(game.win, 3, 1L<<1, key_release, &game);     // Tuş bırakma
    mlx_hook(game.win, 17, 0, close_window, &game);       // Pencere kapatma
    
    // Oyun döngüsünü başlat
    mlx_loop_hook(game.mlx, game_loop, &game);
    mlx_loop(game.mlx);
    
    // Program buraya asla ulaşmaz çünkü mlx_loop sonsuza kadar çalışır
    // Ama yine de temizlik yapalım
    free(game.map);
    free_map(map_lines);
    free_map(actual_map);
    return (0);
} 