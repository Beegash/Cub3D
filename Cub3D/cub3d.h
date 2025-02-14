/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ifozmen <ifozmen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:47:41 by iozmen            #+#    #+#             */
/*   Updated: 2025/02/14 03:39:33 by ifozmen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define MAX_LINE_LENGTH 256
# define WINDOW_WIDTH 1200
# define WINDOW_HEIGHT 750

# include "./mlx/mlx.h"
# include <fcntl.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_map
{
	char		**map_line;
	char		*north_texture;
	char		*south_texture;
	char		*west_texture;
	char		*east_texture;
	int			floor_color[3];
	int			ceiling_color[3];
}				t_map;

typedef struct s_images
{
	char		*south_wall;
	char		*north_wall;
	char		*east_wall;
	char		*west_wall;
	char		*floor;
	char		*ceiling;
	int			floor_texture;
	int			ceiling_texture;
}				t_images;

typedef struct s_keycode
{
	int			w;
	int			a;
	int			s;
	int			d;
	int			left;
	int			right;
}				t_keycode;

typedef struct s_player
{
	double		x;
	double		y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
}				t_player;

typedef struct s_raycast
{
	double		camera_x;
	double		raydir_x;
	double		raydir_y;
	double		sidedist_x;
	double		sidedist_y;
	double		deltadist_x;
	double		deltadist_y;
	int			step_x;
	int			step_y;
	int			side1;
	int			hit;

	int			tex_x;
	int			tex_y;

	double		perp_dist;
	int			height;
	int			start_y;
	int			end_y;
}				t_raycast;

typedef struct s_textures
{
	void		*img;
	int			*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			w;
	int			h;
}				t_textures;

typedef struct s_game
{
	t_map		*map;
	t_images	*img;
	t_keycode	*key;
	t_player	*player;
	t_raycast	*raycast;
	t_textures	*walls[4];

	void		*mlx;
	void		*win;

	void		*img_ptr;
	int			pixel_bits;
	int			line_bytes;
	int			endian;
	int			*mlx_data;

	char		*mapname;
	int			playercount;
	char		playertype;
	int			loc_px;
	int			loc_py;
	int			row;
	int			col;
}				t_game;

typedef struct s_direction
{
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_direction;

int				get_texture(char **map_line, t_map *map);
int				rgb_numbers(char *line, int j, int *rgb);
void			draw_floor_ceiling(t_game *game);
void			perform_raycasting(t_game *game);

int				init_game(t_game *game);
void			init_game_struct(t_game *game);
int				key_press(int keycode, t_game *game);
int				key_release(int keycode, t_game *game);
int				close_window(t_game *game);

void			*ft_memset(void *b, int c, size_t len);
char			*ft_strdup(const char *s1);
char			*ft_strchr(const char *s, int c);
int				ft_strlen(const char *s);
char			**ft_split(char *s, char c);
char			*ft_trimend(const char *s1, const char *set);
int				ft_atoi(const char *str);
char			*ft_strtrim(char *str);
int				is_valid_number(char *str);

char			*replace_tabs_with_spaces(char *line, int spaces_per_tab);
char			**copy_map(char **map);
void			free_map(char **map);
char			**read_map_from_file(char *filename, t_game *game);
void			validate_map(t_game *game);
char			**init_map_lines(int fd, t_game *game);
void			handle_map_error(t_game *game, char **tmpmap, const char *msg);
void			handle_malloc_error(char **new_map, int i, char *error_msg);

void			cleanup_all(t_game *game);
void			cleanup_mlx_content(t_game *game);
void			cleanup_map_content(t_map *map);
int				error_message(char *str, int err);
#endif
