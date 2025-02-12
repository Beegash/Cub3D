NAME = cub3D

SRCS = main.c \
       string_utils.c \
       init_game.c \
       key_hooks.c \
       utils.c \
       texture.c \
       map_validation.c \
       split.c \
       raycasting.c



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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make -C $(MLX_PATH) clean
	rm -f $(OBJS)

fclean: clean
	@make -C $(MLX_PATH) clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
