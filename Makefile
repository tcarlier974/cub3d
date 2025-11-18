NAME = cub3D
CC = cc -g
CFLAGS = -Wall -Wextra -Werror -I./includes
MLX = libmlx_Linux.a
SRC = ./get_next_line/get_next_line.c \
	./get_next_line/get_next_line_utils.c \
	src/main.c \
	src/utils.c \
	src/init.c \
	src/init2.c \
	src/init3.c \
	src/init4.c \
	src/hook.c \
	src/draw.c \
	src/draw2.c \
	src/algo.c \
	src/algo2.c \
	src/check_map.c \
	src/hook2.c \
	src/hook3.c

OBJ = $(SRC:.c=.o)

MACOS_FRAMEWORKS = -framework OpenGL -framework AppKit -framework CoreFoundation
LINUX_FRAMEWORKS = -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz

all: $(NAME)

$(NAME): $(OBJ) $(MLX)
	$(CC) $(OBJ) $(CFLAGS) $(LINUX_FRAMEWORKS) -o $(NAME)  

$(MLX):
	@echo "Building MLX library..."
	@make -C minilibx-linux

%.o: %.c
	$(CC) $(CFLAGS) -Imlx_linux -O3 -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re