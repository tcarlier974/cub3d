/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/02 22:45:39 by igilbert         ###   ########.fr       */
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

# define TEXTURE_COUNT 5
# ifndef WIDTH
#  define WIDTH 1200
# endif
# ifndef HEIGHT
#  define HEIGHT 800
# endif
# ifndef MINIMAP_SIZE
#  define MINIMAP_SIZE 12
# endif
# ifndef ENABLE_MINIMAP
#  define ENABLE_MINIMAP 1
# endif
# ifndef SPRINT_SPEED
#  define SPRINT_SPEED 0.02
# endif
# ifndef WALK_SPEED
#  define WALK_SPEED 0.02
# endif
# ifndef SENSI
#  define SENSI 2.0
# endif
# ifndef ROTA
#  define ROTA 0.01
# endif
# ifndef MARGE_COLLISION
#  define MARGE_COLLISION 0.1
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
	KEY_E,
}	t_keycode;

typedef struct s_algo
{
	int		x;
	int		y;
	int		color;
	int		tex_num;
	int		tex_height;
	int		tex_width;
	int		d;
	int		tex_y;
	double	wall_x;
	int		tex_x;
}	t_algo;

typedef struct s_hook
{
	bool	key_pressed[9];
	bool	mouse_pos[2];
	int		mouse_x;
	int		mouse_y;
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

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

typedef struct s_cube
{
	void	*mlx;
	void	*win;
	t_img	img;
	t_img	texture[TEXTURE_COUNT];
	t_img	sprite;
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
	int		minimap_counter;
	double	old_dir_x;
	double	old_dir_y;
	double	old_plane_x;
	double	old_plane_y;
}	t_cube;

// Function prototypes
void	my_mlx_pixel_put(t_img *img, int x, int y, int color);
void	init_cube(t_cube *cube, const char *map_file);
void	recup_texture(t_cube *cube, const char *map_file);
void	draw_texture(t_cube *cube, int x, int y, t_img texture);
void	init_map(char ***map, const char *file_path, t_cube *cube);
void	raycast(t_cube *cube);
int		key_hook_press(int keycode, t_cube *cube);
int		count_lines(const char *file_path);
int		count_max_col(const char *file_path);
int		bcktrck(t_cube *cube, int x, int y, char **visited);
int		validate_map_closed(t_cube *cube);
// Thread related functions
int		update_game_state(t_cube *cube);

// Add new function prototypes
int		key_release_hook(int keycode, t_cube *cube);
void	process_keys(t_cube *cube);
int		ft_mlx_loop_end(t_cube *cube);
int		rgb_to_int(const char *rgb);
int		color_map(char c);
void	draw_minimap(t_cube *cube);

//algo
void	chose_tex(t_raycast ray, t_algo *var, t_cube *cube);
void	draw_col_conditions(t_cube *cube, t_algo *var, t_raycast ray);
void	draw_col(t_cube *cube, t_algo *var, t_raycast ray);
void	what_to_draw(t_cube *cube, t_algo *var, t_raycast ray);

#endif