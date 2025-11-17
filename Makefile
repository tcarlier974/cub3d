NAME = cub3D
CC = gcc -g
CFLAGS = -I./includes
LIBFT = ./libft/libft.a
MLX = libmlx_intel.a
SRC = ./get_next_line/get_next_line.c \
	./get_next_line/get_next_line_utils.c \
	src/main.c \
	src/utils.c \
	src/init.c \
	src/hook.c \
	src/draw.c \
	src/algo.c \
	src/algo2.c \
	src/check_map.c \
	src/hook2.c \
	src/hook3.c \
	src/init2.c

OBJ = $(SRC:.c=.o)

MACOS_FRAMEWORKS = -framework OpenGL -framework AppKit -framework CoreFoundation

all: $(NAME)

$(NAME): $(OBJ) $(MLX)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(MLX) $(MACOS_FRAMEWORKS)

$(LIBFT):
	make -C libft

$(MLX):
	@echo "Building MLX library..."
	@make -C mlx

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re