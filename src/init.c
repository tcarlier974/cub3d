/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:43:21 by tcarlier          #+#    #+#             */
/*   Updated: 2025/10/24 19:19:25 by tcarlier         ###   ########.fr       */
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
	int		i = 0;
	int		j = 0;

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
	i++;
	textures[4] = strdup("./textures/door.xpm");
	if (!textures[4])
	{
		fprintf(stderr, "Failed to allocate memory for door texture path\n");
		exit(EXIT_FAILURE);
	}
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
	*map = malloc(sizeof(char *) * (count_lines(file_path) + 1));
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
				cube->player_x = (float)i + 0.5;
				cube->player_y = (float)line_count + 0.5;
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
