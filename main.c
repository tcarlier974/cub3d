/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/06/05 18:26:34 by tcarlier         ###   ########.fr       */
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
	int	i;
	cube->mlx = mlx_init();
	cube->win = mlx_new_window(cube->mlx, WIDTH, HEIGHT, "Cub3D");
	cube->img.img = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	cube->img.addr = mlx_get_data_addr(cube->img.img, &cube->img.bits_per_pixel, &cube->img.line_length, &cube->img.endian);
	cube->img.width = WIDTH;
	cube->img.height = HEIGHT;
	cube->map = NULL;
	cube->map_width = count_max_col(map_file);
	cube->map_height = count_lines(map_file);
	i = 0;
	while (i < 6)
	{
		cube->hook.key_pressed[i] = false;
		i++;
	}
	while (i < 8)
	{
		cube->hook.mouse_pos[i - 6] = false;
		i++;
	}
}

int	rgb_to_int(const char *rgb)
{
	int r, g, b;
	int i = 0;
	while (rgb[i] == ' ')
		i++;
	r = atoi(rgb + i);
	while (rgb[i] >= '0' && rgb[i] <= '9')
		i++;
	i++;
	g = atoi(rgb + i);
	while (rgb[i] >= '0' && rgb[i] <= '9')
		i++;
	i++;
	b = atoi(rgb + i);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		fprintf(stderr, "Invalid RGB value: %s\n", rgb);
		exit(EXIT_FAILURE);
	}
	write(1, "RGB values: ", 12);
	printf("R: %d, G: %d, B: %d\n", r, g, b);
	return (r << 16 | g << 8 | b);
}

void	recup_textures_path(char **textures, const char *map_file, t_cube *cube)
{
	int	fd;

	fd = open(map_file, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "Failed to open map file: %s\n", map_file);
		exit(EXIT_FAILURE);
	}
	char	*line;
	int	i = 0;
	int j = 0;
	write(1, "Recuperation des textures...\n", 28);
	line = get_next_line(fd);
	while (i < TEXTURE_COUNT + 2 && line)
	{
		printf("Processing line: %s", line);
		while (line[0] == '\n' || line[0] == '\0')
		{
			free(line);
			line = get_next_line(fd);
		}
		while (line[j] == ' ')
			j++;
		if (line[j] == 'N')
		{
			j+=2;
			while (line[j] == ' ')
				j++;
			printf("Texture N path: %s\n", line + j);
			textures[0] = strdup(line + j);
			if (textures[0]) 
			{
				char *newline = strchr(textures[0], '\n');
				if (newline)
					*newline = '\0';
				printf("Chemin nettoyé: '%s'\n", textures[0]);
			}
			if (!textures[0])
			{
				fprintf(stderr, "Failed to allocate memory for texture path\n");
				exit(EXIT_FAILURE);
			}
			i++;
		}
		else if (line[j] == 'S')
		{
			j+=2;
			while (line[j] == ' ')
				j++;
			textures[1] = strdup(line + j);
			if (textures[1]) 
			{
				char *newline = strchr(textures[1], '\n');
				if (newline)
					*newline = '\0';
				printf("Chemin nettoyé: '%s'\n", textures[1]);
			}
			if (!textures[1])
			{
				fprintf(stderr, "Failed to allocate memory for texture path\n");
				exit(EXIT_FAILURE);
			}
			i++;
		}
		else if (line[j] == 'E')
		{
			j+=2;
			while (line[j] == ' ')
				j++;
			textures[2] = strdup(line + j);
			if (textures[2]) 
			{
				char *newline = strchr(textures[2], '\n');
				if (newline)
					*newline = '\0';
				printf("Chemin nettoyé: '%s'\n", textures[2]);
			}
			if (!textures[2])
			{
				fprintf(stderr, "Failed to allocate memory for texture path\n");
				exit(EXIT_FAILURE);
			}
			i++;
		}
		else if (line[j] == 'W')
		{
			j+=2;
			while (line[j] == ' ')
				j++;
			textures[3] = strdup(line + j);
			if (textures[3]) 
			{
				char *newline = strchr(textures[3], '\n');
				if (newline)
					*newline = '\0';
				printf("Chemin nettoyé: '%s'\n", textures[3]);
			}
			if (!textures[3])
			{
				fprintf(stderr, "Failed to allocate memory for texture path\n");
				exit(EXIT_FAILURE);
			}
			i++;
		}
		else if (line[j] == 'F')
		{
			j++;
			while (line[j] == ' ')
				j++;
			printf("Floor color: %s\n", line + j);
			cube->floor_color = rgb_to_int(line + j);
			if (cube->floor_color < 0 || cube->floor_color > 0xFFFFFF)
			{
				fprintf(stderr, "Invalid floor color value: %d\n", cube->floor_color);
				exit(EXIT_FAILURE);
			}
			printf("Floor color: %d\n", cube->floor_color);
			i++;
		}
		else if (line[j] == 'C')
		{
			j++;
			while (line[j] == ' ')
				j++;
			cube->ceiling_color = rgb_to_int(line + j);
			if (cube->ceiling_color < 0 || cube->ceiling_color > 0xFFFFFF)
			{
				fprintf(stderr, "Invalid ceiling color value: %d\n", cube->ceiling_color);
				exit(EXIT_FAILURE);
			}
			printf("Ceiling color: %d\n", cube->ceiling_color);
			i++;
		}
		else
		{
			fprintf(stderr, "Unknown texture identifier: %c\n", line[j]);
			exit(EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(fd);
		j = 0;
	}
	close(fd);
	if (i < TEXTURE_COUNT)
	{
		fprintf(stderr, "Not enough textures found in the map file: %s\n", map_file);
		exit(EXIT_FAILURE);
	}
}

void	recup_texture(t_cube *cube, const char *map_file)
{
	int	i = 0;
	char	**textures;

	textures = malloc(sizeof(char *) * TEXTURE_COUNT);
	if (!textures)
	{
		fprintf(stderr, "Failed to allocate memory for textures\n");
		exit(EXIT_FAILURE);
	}
	write(1, "Recuperation des textures...\n", 28);
	recup_textures_path(textures, map_file, cube);
	write(1, "Textures recuperées.\n", 22);
	while (i < TEXTURE_COUNT)
	{
		if (!textures[i])
		{
			fprintf(stderr, "Texture path not found for index %d\n", i);
			exit(EXIT_FAILURE);
		}
		printf("Loading texture %d: %s\n", i, textures[i]);
		cube->texture[i].img = NULL;
		cube->texture[i].bits_per_pixel = 0;
		cube->texture[i].line_length = 0;
		cube->texture[i].endian = 0;
		cube->texture[i].width = 0;
		cube->texture[i].height = 0;
		cube->texture[i].img = mlx_xpm_file_to_image(cube->mlx, textures[i], &cube->texture[i].width, &cube->texture[i].height);
		if (!cube->texture[i].img)
		{
			fprintf(stderr, "Failed to load texture: %s\n", textures[i]);
			exit(EXIT_FAILURE);
		}
		free(textures[i]);
		cube->texture[i].addr = mlx_get_data_addr(cube->texture[i].img, &cube->texture[i].bits_per_pixel, &cube->texture[i].line_length, &cube->texture[i].endian);
		printf("Texture %d loaded: %dx%d\n", i, cube->texture[i].width, cube->texture[i].height);
		i++;
	}
	free(textures);
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
	int		j = 0;

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
	while (j < 6)
	{
		while(line[0] == '\n')
		{
			free(line);
			line = get_next_line(fd);
		}
		if (line[0] != '\n' && line[0] != '\0')
		{
			j++;
		}
		free(line);
		line = get_next_line(fd);
	}
	while (line)
	{
		i = 0;
		(*map)[line_count] = malloc(sizeof(char *) * strlen(line) + 1);
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
			else if (line[i] > '0')
				(*map)[line_count][i] = line[i];
			else
				(*map)[line_count][i] = '0';
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
		// || sqrt((ray->side_dist_x * ray->side_dist_x) + (ray->side_dist_y * ray->side_dist_y)) > 4.0
		if (cube->map[ray->map_y][ray->map_x] != '0')
			ray->hit = 1;
	}
}

void	draw_minimap(t_cube *cube)
{
	int x, y;
	int map_x, map_y;

	for (map_y = 0; map_y < cube->map_height; map_y++)
	{
		for (map_x = 0; map_x < cube->map_width; map_x++)
		{
			if (cube->map[map_y][map_x] == '1')
			{
				for (y = 0; y < MINIMAP_SIZE; y++)
				{
					for (x = 0; x < MINIMAP_SIZE; x++)
					{
						my_mlx_pixel_put(&cube->img, map_x * MINIMAP_SIZE + x, map_y * MINIMAP_SIZE + y, 0x808080);
					}
				}
			}
			else if (cube->map[map_y][map_x] == '2')
			{
				for (y = 0; y < MINIMAP_SIZE; y++)
				{
					for (x = 0; x < MINIMAP_SIZE; x++)
					{
						my_mlx_pixel_put(&cube->img, map_x * MINIMAP_SIZE + x, map_y * MINIMAP_SIZE + y, 0xFF0000);
					}
				}
			}
			else if (cube->map[map_y][map_x] == '0')
			{
				for (y = 0; y < MINIMAP_SIZE; y++)
				{
					for (x = 0; x < MINIMAP_SIZE; x++)
					{
						my_mlx_pixel_put(&cube->img, map_x * MINIMAP_SIZE + x, map_y * MINIMAP_SIZE + y, 0x000000);
					}
				}
			}
		}
	}
}

void	draw_position(t_cube *cube)
{
	int x, y;
	int player_x = (int)cube->player_x;
	int player_y = (int)cube->player_y;

	for (y = -MINIMAP_SIZE / 2; y < MINIMAP_SIZE / 2; y++)
	{
		for (x = -MINIMAP_SIZE / 2; x < MINIMAP_SIZE / 2; x++)
		{
			if (x == 0 && y == 0)
				my_mlx_pixel_put(&cube->img, player_x * MINIMAP_SIZE + x + MINIMAP_SIZE / 2, player_y * MINIMAP_SIZE + y + MINIMAP_SIZE / 2, 0xFFFFFF);
			else
				my_mlx_pixel_put(&cube->img, player_x * MINIMAP_SIZE + x + MINIMAP_SIZE / 2, player_y * MINIMAP_SIZE + y + MINIMAP_SIZE / 2, 0xFF0000);
		}
	}
}

void raycast(t_cube *cube)
{
	t_raycast ray;
	int x;
	int	y;
	int	color;
	int tex_num = 0;

	x = 0;
	while (x < WIDTH)
	{
		init_raycast_values(&ray, cube, x);
		dda_algo(&ray, cube);
		if (cube->map[ray.map_y][ray.map_x] != '0')
		{
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
			y = 0;
		}
		while (y < ray.draw_start)
		{
			my_mlx_pixel_put(&cube->img, x, y, cube->ceiling_color);
			y++;
		}
		while (y < ray.draw_end)
		{
			if (cube->map[ray.map_y][ray.map_x] == '1')
			{
				if (ray.side == 0)
				{
					if (ray.ray_dir_x < 0)
						tex_num = 3;
					else
						tex_num = 2;
				}
				else
				{
					if (ray.ray_dir_y < 0)
						tex_num = 0;
					else
						tex_num = 1;
				}
				int tex_height = cube->texture[tex_num].height;
				int tex_width = cube->texture[tex_num].width;
				int d = y * 256 - HEIGHT * 128 + ray.line_height * 128;
				int tex_y = ((d * tex_height) / ray.line_height) / 256;
				double wall_x;
				if (ray.side == 0)
					wall_x = cube->player_y + ray.perp_wall_dist * ray.ray_dir_y;
				else
					wall_x = cube->player_x + ray.perp_wall_dist * ray.ray_dir_x;
				wall_x -= floor(wall_x);
				int tex_x = (int)(wall_x * (double)tex_width);
				if (tex_x < 0) tex_x += tex_width;
				if ((ray.side == 0 && ray.ray_dir_x < 0) || (ray.side == 1 && ray.ray_dir_y > 0))
					tex_x = tex_width - tex_x - 1;
				if (tex_y < 0) tex_y += tex_height;
				color = *(unsigned int *)(cube->texture[tex_num].addr + (tex_y * cube->texture[tex_num].line_length + tex_x * (cube->texture[tex_num].bits_per_pixel / 8)));
			}
			else if (cube->map[ray.map_y][ray.map_x] == '2')
				color = 0xFF0000;
			if (ray.side == 1)
				color = (color >> 1) & 8355711;
			my_mlx_pixel_put(&cube->img, x, y, color);
			y++;
		}
		while (y < HEIGHT)
		{
			my_mlx_pixel_put(&cube->img, x, y, cube->floor_color);
			y++;
		}
		x++;
		draw_minimap(cube);
		draw_position(cube);
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

int key_hook_press(int keycode, t_hook *hook)
{
	if (keycode == 53)
		hook->key_pressed[KEY_ESC] = true;
	if (keycode == 123)
		hook->key_pressed[KEY_LEFT] = true;
	if (keycode == 124)
		hook->key_pressed[KEY_RIGHT] = true;
	if (keycode == 13)
		hook->key_pressed[KEY_W] = true;
	if (keycode == 0)
		hook->key_pressed[KEY_A] = true;
	if (keycode == 2)
		hook->key_pressed[KEY_D] = true;
	if (keycode == 1)
		hook->key_pressed[KEY_S] = true;
	return (0);
}

int	update_game_state(t_cube *cube)
{
	if (cube->hook.key_pressed[KEY_ESC])
	{
		ft_mlx_loop_end(cube);
		return (0);
	}
	if (cube->hook.key_pressed[KEY_LEFT])
	{
		double old_dir_x = cube->dir_x;
		cube->dir_x = cube->dir_x * cos(-0.09) - cube->dir_y * sin(-0.09);
		cube->dir_y = old_dir_x * sin(-0.09) + cube->dir_y * cos(-0.09);
		double old_plane_x = cube->plane_x;
		cube->plane_x = cube->plane_x * cos(-0.09) - cube->plane_y * sin(-0.09);
		cube->plane_y = old_plane_x * sin(-0.09) + cube->plane_y * cos(-0.09);
	}
	if (cube->hook.key_pressed[KEY_RIGHT])
	{
		double old_dir_x = cube->dir_x;
		cube->dir_x = cube->dir_x * cos(0.09) - cube->dir_y * sin(0.09);
		cube->dir_y = old_dir_x * sin(0.09) + cube->dir_y * cos(0.09);
		double old_plane_x = cube->plane_x;
		cube->plane_x = cube->plane_x * cos(0.09) - cube->plane_y * sin(0.09);
		cube->plane_y = old_plane_x * sin(0.09) + cube->plane_y * cos(0.09);
	}
	if (cube->hook.key_pressed[KEY_W])
	{
		if (cube->map[(int)(cube->player_y + (cube->dir_y * 0.15))] != NULL)
		{
			if(cube->map[(int)(cube->player_y + (cube->dir_y * 0.15))][(int)(cube->player_x)] == '0')
				cube->player_y += (cube->dir_y * 0.09);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x + (cube->dir_x * 0.15))] == '0')
			cube->player_x += (cube->dir_x * 0.09);
    }
	if (cube->hook.key_pressed[KEY_A])
	{
		if (cube->map[(int)(cube->player_y - (cube->dir_y * 0.15))] != NULL)
		{
			if(cube->map[(int)(cube->player_y - (cube->plane_y * 0.15))][(int)(cube->player_x)] == '0')
				cube->player_y -= (cube->plane_y * 0.09);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x - (cube->plane_x * 0.15))] == '0')
			cube->player_x -= (cube->plane_x * 0.09);
	}
	if (cube->hook.key_pressed[KEY_D])
	{
		if (cube->map[(int)(cube->player_y + (cube->plane_y * 0.15))] != NULL)
		{
			if(cube->map[(int)(cube->player_y + (cube->plane_y * 0.15))][(int)(cube->player_x)] == '0')
				cube->player_y += (cube->plane_y * 0.09);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x + (cube->plane_x * 0.15))] == '0')
			cube->player_x += (cube->plane_x * 0.09);
	}
	if (cube->hook.key_pressed[KEY_S])
	{
		if (cube->map[(int)(cube->player_y - (cube->dir_y * 0.15))])
		{
			if(cube->map[(int)(cube->player_y - (cube->dir_y * 0.15))][(int)(cube->player_x)] == '0')
				cube->player_y -= (cube->dir_y * 0.09);
		}
      	if(cube->map[(int)(cube->player_y)][(int)(cube->player_x - (cube->dir_x * 0.15))] == '0')
	  		cube->player_x -= (cube->dir_x * 0.09);
	}
	mlx_destroy_image(cube->mlx, cube->img.img);
	cube->img.img = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	cube->img.addr = mlx_get_data_addr(cube->img.img,
			&cube->img.bits_per_pixel, &cube->img.line_length,
			&cube->img.endian);
	raycast(cube);
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img.img, 0, 0);
	return (0);
}

int key_release_hook(int keycode, t_hook *hook)
{
	if (keycode == 123)
		hook->key_pressed[KEY_LEFT] = false;
	if (keycode == 124)
		hook->key_pressed[KEY_RIGHT] = false;
	if (keycode == 13)
		hook->key_pressed[KEY_W] = false;
	if (keycode == 0)
		hook->key_pressed[KEY_A] = false;
	if (keycode == 2)
		hook->key_pressed[KEY_D] = false;
	if (keycode == 1)
		hook->key_pressed[KEY_S] = false;
	return (0);
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
	
	if (count_lines(av[1]) <= 0 || count_max_col(av[1]) <= 0)
	{
		fprintf(stderr, "Invalid map file: %s\n", av[1]);
		return (1);
	}
	init_cube(&cube, av[1]);
	recup_texture(&cube, av[1]);
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
	mlx_do_key_autorepeaton(cube.mlx);
	mlx_hook(cube.win, 17, 0, ft_mlx_loop_end, &cube);
	mlx_hook(cube.win, 2, 1L << 0, key_hook_press, &cube.hook);
	mlx_hook(cube.win, 3, 1L << 1, key_release_hook, &cube.hook);
	mlx_loop_hook(cube.mlx, update_game_state, &cube);
	mlx_loop(cube.mlx);
	return (0);
}
