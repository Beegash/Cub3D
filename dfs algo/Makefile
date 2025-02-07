NAME = cub3D
SRCS = main.c \
       init_game.c \
       key_hooks.c \
       utils.c \
       texture.c \
       dfc.c \
       get_next_line.c \
       get_next_line_utils.c \
       split.c

OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./mlx
MLX_PATH = ./mlx
MLX_NAME = libmlx.a
MLX_FLAGS = -framework OpenGL -framework AppKit

all: $(MLX_PATH)/$(MLX_NAME) $(NAME)

$(MLX_PATH)/$(MLX_NAME):
	@make -C $(MLX_PATH)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -L$(MLX_PATH) -lmlx $(MLX_FLAGS) -o $(NAME)

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