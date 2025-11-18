/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:43:21 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/18 12:57:24 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	init_cube(t_cube *cube, const char *map_file)
{
	int	i;

	cube->mlx = mlx_init();
	cube->win = mlx_new_window(cube->mlx, WIDTH, HEIGHT, "Cub3D");
	cube->img.img = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	cube->img.addr = mlx_get_data_addr(cube->img.img,
			&cube->img.bits_per_pixel,
			&cube->img.line_length, &cube->img.endian);
	cube->img.width = WIDTH;
	cube->img.height = HEIGHT;
	cube->map = NULL;
	cube->map_width = count_max_col(map_file);
	cube->map_height = count_lines(map_file);
	i = 0;
	while (i < 9)
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

void	recup_textures_path(char **textures, const char *map_file, t_cube *cube)
{
	int		fd;
	int		vars[2];

	vars[0] = 0;
	vars[1] = 0;
	fd = open(map_file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd(2, "Failed to open map file\n");
		exit(EXIT_FAILURE);
	}
	read_texture_lines(fd, textures, cube, vars);
	close(fd);
	textures[4] = gnl_ft_strdup("./textures/porte.xpm");
	if (!textures[4] || vars[0] < 6)
	{
		ft_putstr_fd(2, "Error loading textures\n");
		exit(EXIT_FAILURE);
	}
}

void	recup_texture(t_cube *cube, const char *map_file)
{
	int		i;
	char	**textures;

	i = 0;
	textures = malloc(sizeof(char *) * TEXTURE_COUNT);
	if (!textures)
	{
		ft_putstr_fd(2, "Failed to allocate memory for textures\n");
		exit(EXIT_FAILURE);
	}
	recup_textures_path(textures, map_file, cube);
	while (i < TEXTURE_COUNT)
	{
		texture_recup(cube, &textures, i);
		i++;
	}
	free(textures);
}

void	init_map(char ***map, const char *file_path, t_cube *cube)
{
	t_init_tool	t;

	t.line_count = 0;
	t.j = 0;
	t.k = 0;
	t.l = 0;
	t.i = 0;
	t.fd = open(file_path, O_RDONLY);
	if (t.fd < 0)
	{
		perror("Failed to open map file");
		return ;
	}
	*map = malloc(sizeof(char *) * (cube->map_height + 1));
	if (!*map)
	{
		close(t.fd);
		return ;
	}
	t.line = get_next_line(t.fd);
	skip_texture_lines(&t);
	process_map_lines(&t, map, cube);
	close(t.fd);
}
