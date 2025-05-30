NAME = cub3D
CC = gcc
CFLAGS = -I./includes
LIBFT = ./libft/libft.a
MLX = libmlx.a
SRC = ./get_next_line/get_next_line.c \
	./get_next_line/get_next_line_utils.c \
	main.c \
	utils.c \

OBJ = $(SRC:.c=.o)

# Add macOS framework flags
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