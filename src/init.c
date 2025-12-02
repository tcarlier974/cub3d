/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:43:21 by tcarlier          #+#    #+#             */
/*   Updated: 2025/12/02 17:34:39 by igilbert         ###   ########.fr       */
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
	cube->floor_color = -1;
	cube->ceiling_color = -1;
	cube->player_exists = 0;
	cube->wrongmap = 0;
	cube->player_x = -1;
	cube->player_y = -1;
	i = 0;
	while (i < TEXTURE_COUNT)
		cube->texture[i++].img = NULL;
	i = 0;
	while (i < 9)
		cube->hook.key_pressed[i++] = false;
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
		ft_putstr_fd(2, "Error : Failed to open map file\n");
		ft_exit(1, cube, NULL);
	}
	read_texture_lines(fd, textures, cube, vars);
	close(fd);
	textures[4] = gnl_ft_strdup("./textures/porte.xpm");
	if (!textures[4] || vars[0] < 6)
	{
		ft_putstr_fd(2, "Error loading textures\n");
		ft_exit(1, cube, NULL);
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
		ft_putstr_fd(2, "Error : Failed to allocate memory for textures\n");
		ft_exit(1, cube, NULL);
	}
	while (i < TEXTURE_COUNT)
		textures[i++] = NULL;
	recup_textures_path(textures, map_file, cube);
	i = 0;
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
