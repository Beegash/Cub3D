NAME = cub3D

SRCS = main.c \
       ft_str_funcs.c \
       ft_split.c \
       ft_str_utils.c \
       game_init.c \
       game_start.c \
       game_components.c \
       game_components_utils.c \
       game_movement.c \
       key_hooks.c \
       map_handle.c \
       map_operations.c \
       map_texture.c \
       map_texture_utils.c \
       map_algorithms.c \
       map_validation.c \
       map_read.c \
       ray_cast_funcs.c \
       ray_cast_draw.c \
       ray_cast_draw_utils.c \
       cleanup.c

GNL_PATH = ./get_next_line/get_next_line.c \
           ./get_next_line/get_next_line_utils.c
OBJS = $(SRCS:.c=.o) $(GNL_PATH:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./mlx -I./$(GNL_DIR)
MLX_PATH = ./mlx
MLX_NAME = libmlx.a
MLX_FLAGS = -L$(MLX_PATH) -lmlx -lXext -lX11 -lm -lz 

all: $(MLX_PATH)/$(MLX_NAME) $(NAME)

$(MLX_PATH)/$(MLX_NAME):
	@make -C $(MLX_PATH)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -L$(MLX_PATH) -lmlx $(MLX_FLAGS) -lm -o $(NAME) 

clean:
	@make -C $(MLX_PATH) clean
	rm -f $(OBJS)

fclean: clean
	@make -C $(MLX_PATH) clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
