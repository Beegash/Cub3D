 NAME = texture
SRCS = texture.c \
       dfc.c \
       get_next_line.c \
       get_next_line_utils.c \
       split.c
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME):
	gcc $(FLAGS) $(SRCS) -o $(NAME)

clean:
	rm -f $(NAME)

re: clean all

.PHONY: all clean re