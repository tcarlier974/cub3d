/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/06/07 00:56:10 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDE_H
# define INCLUDE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <string.h>
# include <pthread.h>
# include "mlx.h"
# include "../get_next_line/get_next_line.h"
# include <stdbool.h>

# define TEXTURE_COUNT 4
# define RENDER_FPS 30
# ifndef WIDTH
#  define WIDTH 1600
# endif
# ifndef HEIGHT
#  define HEIGHT 1200
# endif
# define MINIMAP_SIZE 5
# ifndef SPRINT_SPEED
#  define SPRINT_SPEED 0.2
# endif
# ifndef WALK_SPEED
#  define WALK_SPEED 0.09
# endif

typedef enum e_keycode
{
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ESC,
	KEY_SHIFT,
}	t_keycode;

typedef struct s_hook
{
	bool key_pressed[8];
	bool mouse_pos[2];
	int mouse_x;
	int mouse_y;
}	t_hook;

typedef struct s_raycast
{
	int		hit;
	int		side;
	int		map_x;
	int		map_y;
	double	perp_wall_dist;
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	side_dist_x;
	double	side_dist_y;
	int		step_x;
	int		step_y;
	int		line_height;
	int		draw_start;
	int		draw_end;
}	t_raycast;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}	t_img;

typedef struct s_cube
{
	void	*mlx;
	void	*win;
	t_img	img;
	t_img	texture[TEXTURE_COUNT];
	int		ceiling_color;
	int		floor_color;
	char	**map;
	int		map_width;
	int		map_height;
	double	player_x;
	double	player_y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	t_hook	hook;
	double	move_speed;
}	t_cube;

// Function prototypes
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);
void	init_cube(t_cube *cube, const char *map_file);
void	recup_texture(t_cube *cube, const char *map_file);
void	draw_texture(t_cube *cube, int x, int y, t_img texture);
void	init_map(char ***map, const char *file_path, t_cube *cube);
void	raycast(t_cube *cube);
int     key_hook_press(int keycode, t_cube *cube);
int     count_lines(const char *file_path);
int     count_max_col(const char *file_path);

// Thread related functions
int		update_game_state(t_cube *cube);

// Add new function prototypes
int		key_release_hook(int keycode, t_hook *hook);
void	process_keys(t_cube *cube);
int     ft_mlx_loop_end(t_cube *cube);

#endif