/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/05/30 17:08:20 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/include.h"


void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	init_cube(t_cube *cube, const char *map_file)
{
	cube->mlx = mlx_init();
	cube->win = mlx_new_window(cube->mlx, WIDTH, HEIGHT, "Cub3D");
	cube->img.img = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	cube->img.addr = mlx_get_data_addr(cube->img.img, &cube->img.bits_per_pixel, &cube->img.line_length, &cube->img.endian);
	cube->img.width = WIDTH;
	cube->img.height = HEIGHT;
	cube->map = NULL;
	cube->map_width = count_col(map_file);
	cube->map_height = count_lines(map_file);
}

void	recup_texture(t_cube *cube)
{
	int	i = 0;
	char	*textures[] = {
		"./textures/texture2.xpm"
	};
	cube->texture[i].img = mlx_xpm_file_to_image(cube->mlx, textures[i], &cube->texture[i].width, &cube->texture[i].height);
	if (!cube->texture[i].img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", textures[i]);
		exit(EXIT_FAILURE);
	}
	cube->texture[i].addr = mlx_get_data_addr(cube->texture[i].img, &cube->texture[i].bits_per_pixel, &cube->texture[i].line_length, &cube->texture[i].endian);
	printf("Texture %d loaded: %dx%d\n", i, cube->texture[i].width, cube->texture[i].height);
	i++;
}
void	draw_texture(t_cube *cube, int x, int y, t_img texture)
{
	int		i;
	int		j;
	unsigned int	color;

	for (j = 0; j < texture.height; j++)
	{
		for (i = 0; i < texture.width; i++)
		{
			color = *(unsigned int *)(texture.addr + (j * texture.line_length + i * (texture.bits_per_pixel / 8)));
			my_mlx_pixel_put(&cube->img, x + i, y + j, color);
		}
	}
}

void init_map(char ***map, const char *file_path, t_cube *cube)
{
	int		fd;
	char	*line;
	int		line_count = 0;
	int		i = 0;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
	{
		perror("Failed to open map file");
		return;
	}
	*map = malloc(sizeof(char *) * count_lines(file_path));
	if (!*map)
		return;
	line = get_next_line(fd);
	while (line)
	{
		i = 0;
		(*map)[line_count] = malloc(strlen(line) + 1);
		if (!(*map)[line_count])
		{
			free(line);
			return;
		}
		while(line[i] != '\0' && line[i] != '\n')
		{
			if (line[i] == 'N' || line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
			{
				(*map)[line_count][i] = '0';
				cube->dir_x = (line[i] == 'E') - (line[i] == 'W');
				cube->dir_y = (line[i] == 'S') - (line[i] == 'N');
				cube->plane_x = -cube->dir_y * 0.66;
				cube->plane_y = cube->dir_x * 0.66;
				cube->player_x = (float)i;
				cube->player_y = (float)line_count;
			}
			else if (line[i] == ' ')
				(*map)[line_count][i] = '0';
			else
			(*map)[line_count][i] = line[i];
			i++;
		}
		(*map)[line_count][i] = '\0';
		line_count++;
		free(line);
		line = get_next_line(fd);
	}
	(*map)[line_count] = NULL;
	close(fd);
}

void	second_init_ray(t_raycast *ray, t_cube *cube, int x)
{
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (cube->player_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - cube->player_y) * ray->delta_dist_y;
	}
}
	
void	init_raycast_values(t_raycast *ray, t_cube *cube, int x)
{
	ray->hit = 0;
	ray->map_x = (int)cube->player_x;
	ray->map_y = (int)cube->player_y;
	ray->camera_x = 2 * (double)x / (double)WIDTH - 1;
	ray->ray_dir_x = cube->dir_x + cube->plane_x * ray->camera_x;
	ray->ray_dir_y = cube->dir_y + cube->plane_y * ray->camera_x;
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (cube->player_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - cube->player_x) * ray->delta_dist_x;
	}
	second_init_ray(ray, cube, x);
}

void	dda_algo(t_raycast *ray, t_cube *cube)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (cube->map[ray->map_y][ray->map_x] != '0')
			ray->hit = 1;
	}
}

void raycast(t_cube *cube)
{
	t_raycast ray;
	int x;
	int	y;
	int	color;

	x = 0;
	while (x < WIDTH)
	{
		init_raycast_values(&ray, cube, x);
		dda_algo(&ray, cube);
		if (ray.side == 0) 
			ray.perp_wall_dist = (ray.side_dist_x - ray.delta_dist_x);
     	else
			ray.perp_wall_dist = (ray.side_dist_y - ray.delta_dist_y);
		ray.line_height = (int)(HEIGHT / ray.perp_wall_dist);
		ray.draw_start = (-ray.line_height / 2) + (HEIGHT / 2);
      	if(ray.draw_start < 0)
	  		ray.draw_start = 0;
      	ray.draw_end = (ray.line_height / 2) + (HEIGHT / 2);
      	if(ray.draw_end >= HEIGHT)
	  		ray.draw_end = HEIGHT - 1;
		y = ray.draw_start;
		while (y < ray.draw_end)
		{
			if (cube->map[ray.map_y][ray.map_x] == '1')
			{
				int tex_height = cube->texture[0].height;
				int tex_width = cube->texture[0].width;
				int d = y * 256 - HEIGHT * 128 + ray.line_height * 128;
				int tex_y = ((d * tex_height) / ray.line_height) / 256;
				double wall_x;
				if (ray.side == 0)
					wall_x = cube->player_y + ray.perp_wall_dist * ray.ray_dir_y;
				else
					wall_x = cube->player_x + ray.perp_wall_dist * ray.ray_dir_x;
				wall_x -= floor(wall_x);
				int tex_x = (int)(wall_x * (double)tex_width);
				if (ray.side == 0 && ray.ray_dir_x > 0)
					tex_x = tex_width - tex_x - 1;
				if (ray.side == 1 && ray.ray_dir_y < 0)
					tex_x = tex_width - tex_x - 1;
				if (tex_x < 0) tex_x += tex_width;
				if (tex_y < 0) tex_y += tex_height;
				color = *(unsigned int *)(cube->texture[0].addr + (tex_y * cube->texture[0].line_length + tex_x * (cube->texture[0].bits_per_pixel / 8)));
			}
			else if (cube->map[ray.map_y][ray.map_x] == '2')
				color = 0xFF0000;
			if (ray.side == 1)
				color = (color >> 1) & 8355711;
			my_mlx_pixel_put(&cube->img, x, y, color);
			y++;
		}
		x++;
	}

}

int ft_mlx_loop_end(t_cube *cube)
{
	mlx_destroy_window(cube->mlx, cube->win);
	mlx_destroy_image(cube->mlx, cube->img.img);
	for (int i = 0; i < TEXTURE_COUNT; i++)
	{
		if (cube->texture[i].img)
			mlx_destroy_image(cube->mlx, cube->texture[i].img);
	}
	free(cube->map);
	exit(0);
	return (0);
}

int key_hook(int keycode, t_cube *cube)
{
	if (keycode == 53)
	{
		ft_mlx_loop_end(cube);
		return (0);
	}
	if (keycode == 123)
	{
		double old_dir_x = cube->dir_x;
		cube->dir_x = cube->dir_x * cos(-0.09) - cube->dir_y * sin(-0.09);
		cube->dir_y = old_dir_x * sin(-0.09) + cube->dir_y * cos(-0.09);
		double old_plane_x = cube->plane_x;
		cube->plane_x = cube->plane_x * cos(-0.09) - cube->plane_y * sin(-0.09);
		cube->plane_y = old_plane_x * sin(-0.09) + cube->plane_y * cos(-0.09);
	}
	if (keycode == 124)
	{
		double old_dir_x = cube->dir_x;
		cube->dir_x = cube->dir_x * cos(0.09) - cube->dir_y * sin(0.09);
		cube->dir_y = old_dir_x * sin(0.09) + cube->dir_y * cos(0.09);
		double old_plane_x = cube->plane_x;
		cube->plane_x = cube->plane_x * cos(0.09) - cube->plane_y * sin(0.09);
		cube->plane_y = old_plane_x * sin(0.09) + cube->plane_y * cos(0.09);
	}
	if (keycode == 13)
	{
		if (cube->map[(int)(cube->player_y + (cube->dir_y * 0.09))] != NULL)
		{
			if(cube->map[(int)(cube->player_y + (cube->dir_y * 0.09))][(int)(cube->player_x)])
				cube->player_y += (cube->dir_y * 0.09);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x + (cube->dir_x * 0.09))])
			cube->player_x += (cube->dir_x * 0.09);
    }
	if (keycode == 0)
	{
		if (cube->map[(int)(cube->player_y - (cube->dir_y * 0.09))] != NULL)
		{
			if(cube->map[(int)(cube->player_y - (cube->dir_y * 0.09))][(int)(cube->player_x)])
				cube->player_y -= (cube->plane_y * 0.09);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x - (cube->dir_x * 0.09))])
			cube->player_x -= (cube->plane_x * 0.09);
	}
	if (keycode == 2)
	{
		if (cube->map[(int)(cube->player_y + (cube->plane_y * 0.09))] != NULL)
		{
			if(cube->map[(int)(cube->player_y + (cube->plane_y * 0.09))][(int)(cube->player_x)])
				cube->player_y += (cube->plane_y * 0.09);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x + (cube->plane_x * 0.09))])
			cube->player_x += (cube->plane_x * 0.09);
	}
	if (keycode == 1)
	{
		if (cube->map[(int)(cube->player_y - (cube->dir_y * 0.09))])
		{
			if(cube->map[(int)(cube->player_y - (cube->dir_y * 0.09))][(int)(cube->player_x)])
				cube->player_x -= (cube->dir_x * 0.09);
		}
      	if(cube->map[(int)(cube->player_y)][(int)(cube->player_x - (cube->dir_x * 0.09))])
	  		cube->player_y -= (cube->dir_y * 0.09);
	}
	printf("Player at x = %.2f  y = %.2f\n", cube->player_x, cube->player_y);
	printf("keycode = %d\n", keycode);
	mlx_destroy_image(cube->mlx, cube->img.img);
	cube->img.img = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	cube->img.addr = mlx_get_data_addr(cube->img.img,
			&cube->img.bits_per_pixel, &cube->img.line_length,
			&cube->img.endian);
	raycast(cube);
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img.img, 0, 0);
}

int key_release_hook(int keycode, t_cube *cube)
{
}

void process_keys(t_cube *cube)
{

}

int	main(int ac, char **av)
{
	t_cube cube;
	int i = 0;

	if (ac != 2)
	{
		fprintf(stderr, "Usage: %s <map_file>\n", av[0]);
		return (1);
	}
	
	if (count_lines(av[1]) <= 0 || count_col(av[1]) <= 0)
	{
		fprintf(stderr, "Invalid map file: %s\n", av[1]);
		return (1);
	}
	init_cube(&cube, av[1]);
	recup_texture(&cube);
	init_map(&cube.map, av[1], &cube);
	printf("Carte chargée:\n");
	for (int y = 0; y < cube.map_height; y++) {
		if (cube.map[y])
			printf("[%d] %s\n", y, cube.map[y]);
	}
	printf("map[6][5] = %c\n", cube.map[6][5]);
	printf("Player initialized at position: %.2f, %.2f\n", cube.player_x, cube.player_y);
	printf("Direction: (%.2f, %.2f)\n", cube.dir_x, cube.dir_y);
	raycast(&cube);
	mlx_put_image_to_window(cube.mlx, cube.win, cube.img.img, 0, 0);
	mlx_hook(cube.win, 17, 0, ft_mlx_loop_end, &cube);
	mlx_hook(cube.win, 2, 1L << 0, key_hook, &cube);
	mlx_hook(cube.win, 3, 1L << 1, key_release_hook, &cube);
	mlx_loop(cube.mlx);
	return (0);
}
