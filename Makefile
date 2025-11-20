NAME = cub3D
CC = cc -g
CFLAGS = -Wall -Wextra -Werror -I./includes
MLX = libmlx_Linux.a
SRC = ./get_next_line/get_next_line.c \
	./get_next_line/get_next_line_utils.c \
	src/main.c \
	src/utils.c \
	src/utils2.c \
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
LINUX_FRAMEWORKS = -L./minilibx-linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz

MLX_FLAGS = $(LINUX_FRAMEWORKS)
MLX_INC = -Imlx_linux
MLX_BUILD = @make -C minilibx-linux

all: linux

linux: $(NAME)

intel: MLX_FLAGS = libmlx_intel.a $(MACOS_FRAMEWORKS)
intel: MLX_INC = -I.
intel: MLX_BUILD = 
intel: $(NAME)

silicon: MLX_FLAGS = libmlx_sillicon.a $(MACOS_FRAMEWORKS)
silicon: MLX_INC = -I.
silicon: MLX_BUILD = 
silicon: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(MLX_INC) -O3 -c $< -o $@

$(NAME): $(OBJ)
	$(MLX_BUILD)
	$(CC) $(OBJ) $(CFLAGS) $(MLX_FLAGS) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re linux intel silicon