/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:43:21 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/17 05:09:50 by igilbert         ###   ########.fr       */
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

void	recup_textures_path(char **textures, const char *map_file, t_cube *cube)
{
	int		fd;
	char	*line;
	int		i;
	int		j;
	char	*newline;

	i = 0;
	j = 0;
	fd = open(map_file, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "Failed to open map file: %s\n", map_file);
		exit(EXIT_FAILURE);
	}
	write(1, "Recuperation des textures...\n", 28);
	line = get_next_line(fd);
	while (i < TEXTURE_COUNT + 1 && line)
	{
		while (line[0] == '\n' || line[0] == '\0')
		{
			free(line);
			line = get_next_line(fd);
		}
		while (line[j] == ' ')
			j++;
		if (line[j] == 'N')
		{
			j += 2;
			while (line[j] == ' ')
				j++;
			printf("Texture N path: %s\n", line + j);
			textures[0] = strdup(line + j);
			if (textures[0])
			{
				newline = strchr(textures[0], '\n');
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
			j += 2;
			while (line[j] == ' ')
				j++;
			textures[1] = strdup(line + j);
			if (textures[1])
			{
				newline = strchr(textures[1], '\n');
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
			j += 2;
			while (line[j] == ' ')
				j++;
			textures[2] = strdup(line + j);
			if (textures[2])
			{
				newline = strchr(textures[2], '\n');
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
			j += 2;
			while (line[j] == ' ')
				j++;
			textures[3] = strdup(line + j);
			if (textures[3])
			{
				newline = strchr(textures[3], '\n');
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
				fprintf(stderr, "Invalid floor color value: %d\n",
					cube->floor_color);
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
				fprintf(stderr, "Invalid ceiling color value: %d\n",
					cube->ceiling_color);
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
	i++;
	textures[4] = strdup("./textures/porte.xpm");
	if (!textures[4])
	{
		fprintf(stderr, "Failed to allocate memory for door texture path\n");
		exit(EXIT_FAILURE);
	}
	if (i < TEXTURE_COUNT)
	{
		fprintf(stderr, "Not enough textures found in the map file: %s\n",
			map_file);
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
		fprintf(stderr, "Failed to allocate memory for textures\n");
		exit(EXIT_FAILURE);
	}
	write(1, "Recuperation des textures...\n", 28);
	recup_textures_path(textures, map_file, cube);
	write(1, "Textures recuperées.\n", 22);
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
	while (t.j < 6 && t.line)
	{
		while (t.line && (t.line[0] == '\n' || t.line[0] == '\0'))
		{
			free(t.line);
			t.line = get_next_line(t.fd);
		}
		if (!t.line)
			break ;
		if (t.line[0] != '\n' && t.line[0] != '\0')
			t.j++;
		free(t.line);
		t.line = get_next_line(t.fd);
	}
	while (t.line)
	{
		t.i = 0;
		t.l = 0;
		t.len = strlen(t.line);
		(*map)[t.line_count] = malloc(sizeof(char) * (cube->map_width + 1));
		if (!(*map)[t.line_count])
		{
			free(t.line);
			while (t.k < t.line_count)
			{
				free((*map)[t.k]);
				t.k++;
			}
			free(*map);
			close(t.fd);
			return ;
		}
		while (t.l < cube->map_width)
		{
			(*map)[t.line_count][t.l] = '0';
			t.l++;
		}
		(*map)[t.line_count][cube->map_width] = '\0';
		while (t.i < (int)t.len && t.i < cube->map_width && t.line[t.i] != '\n')
		{
			t.ch = t.line[t.i];
			if (t.ch == 'N' || t.ch == 'S' || t.ch == 'E' || t.ch == 'W')
			{
				(*map)[t.line_count][t.i] = '0';
				cube->dir_x = (t.ch == 'E') - (t.ch == 'W');
				cube->dir_y = (t.ch == 'S') - (t.ch == 'N');
				cube->plane_x = -cube->dir_y * 0.66;
				cube->plane_y = cube->dir_x * 0.66;
				cube->player_x = (double)t.i + 0.5;
				cube->player_y = (double)t.line_count + 0.5;
			}
			else if (t.ch == ' ')
				(*map)[t.line_count][t.i] = '0';
			else if (t.ch > '0')
				(*map)[t.line_count][t.i] = t.ch;
			else
				(*map)[t.line_count][t.i] = '0';
			t.i++;
		}
		t.line_count++;
		free(t.line);
		t.line = get_next_line(t.fd);
	}
	(*map)[t.line_count] = NULL;
	close(t.fd);
}
