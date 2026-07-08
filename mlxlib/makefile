NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -O2
MLX_DIR = mlx
MLX_LIB = $(MLX_DIR)/libmlx.a
SRCS = main.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(MLX_LIB):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(OBJS) $(MLX_LIB)
	$(CC) $(OBJS) $(MLX_LIB) -lXext -lX11 -lm -lbsd -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re run
