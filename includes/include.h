/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   include.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/12/02 14:45:33 by tcarlier         ###   ########.fr       */
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
# include <sys/time.h>
# include "mlx.h"
# include "../get_next_line/get_next_line.h"
# include <stdbool.h>

# define TEXTURE_COUNT 5
# ifndef WIDTH
#  define WIDTH 1280
# endif
# ifndef HEIGHT
#  define HEIGHT 720
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

typedef struct s_init_tool
{
	int		fd;
	char	*line;
	char	ch;
	int		line_count;
	int		j;
	int		k;
	int		l;
	int		i;
	size_t	len;
}	t_init_tool;

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

typedef struct s_colcmp
{
	int		fd;
	char	*line;
	int		max;
	int		i;
	int		j;
}	t_colcmp;

typedef struct s_gs
{
	double	old_dir_x;
	double	old_dir_y;
	double	old_plane_x;
	double	old_plane_y;
	double	new_x;
	double	new_y;
	int		front_x;
	int		front_y;
}	t_gs;

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
void	ft_putstr_fd(int fd, char *str);
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
void	texture_recup(t_cube *cube, char ***textures, int i);
// Thread related functions
int		update_game_state(t_cube *cube);
void	do_key_left(t_gs gs, t_cube *cube);
void	do_key_right(t_gs gs, t_cube *cube);
void	do_key_w(t_gs gs, t_cube *cube);
void	do_key_a(t_gs gs, t_cube *cube);
void	do_key_s(t_gs gs, t_cube *cube);
void	do_key_d(t_gs gs, t_cube *cube);
void	do_key_e(t_gs gs, t_cube *cube);
bool	can_move_y(t_cube *cube, t_gs gs);
bool	can_move_x(t_cube *cube, t_gs gs);

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
void	what_to_draw(t_cube *cube, t_algo *var, t_raycast *ray);
void	dda_algo_conditions(t_cube *cube, t_raycast *ray);
void	hooking(t_gs *gs, t_cube *cube);

// Init helper functions
void	skip_texture_lines(t_init_tool *t);
int		alloc_map_line(t_init_tool *t, char ***map, t_cube *cube);
void	init_map_process_char(t_init_tool *t, char ***map, t_cube *cube);
void	init_map_parse_line(t_init_tool *t, char ***map, t_cube *cube);
void	process_map_lines(t_init_tool *t, char ***map, t_cube *cube);
void	read_texture_lines(int fd, char **textures, t_cube *cube, int *vars);
void	parse_texture_identifier(char *line, char **textures,
			t_cube *cube, int *vars);
void	recup_textures_path(char **textures, const char *map_file,
			t_cube *cube);
void	process_texture_wall(char *line, char **textures, int *vars);

// Draw helper functions
void	init_minimap_vars(int *vars);
void	draw_minimap_pixel(t_cube *cube, int base_x, int base_y, int color);
void	draw_minimap_grid(t_cube *cube, int *vars);
void	draw_minimap_player(t_cube *cube, int *vars);
void	draw_minimap_direction(t_cube *cube, int px, int py, int *dir_vars);
char	*ft_strchr(const char *s, int c);
int		ft_strlen(char *str);
int		ft_atoi(const char *str);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

#endif