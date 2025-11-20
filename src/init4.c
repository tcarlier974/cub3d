/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 06:20:00 by igilbert          #+#    #+#             */
/*   Updated: 2025/11/20 17:36:57 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

char	*process_texture_line(char *line, int *j)
{
	char	*texture;
	char	*newline;

	*j += 2;
	while (line[*j] == ' ')
		(*j)++;
	texture = gnl_ft_strdup(line + *j);
	if (texture)
	{
		newline = ft_strchr(texture, '\n');
		if (newline)
			*newline = '\0';
	}
	if (!texture)
	{
		ft_putstr_fd(2, "Failed to allocate memory for texture path\n");
		exit(EXIT_FAILURE);
	}
	return (texture);
}

void	process_floor_color(char *line, int *j, t_cube *cube, int *i)
{
	if (cube->floor_color != -1)
	{
		ft_putstr_fd(2, "Error: Duplicate F identifier\n");
		exit(EXIT_FAILURE);
	}
	(*j)++;
	while (line[*j] == ' ')
		(*j)++;
	cube->floor_color = rgb_to_int(line + *j);
	if (cube->floor_color == -1)
	{
		ft_putstr_fd(2, "Error: Invalid floor color value. Using default Brown.\n");
		cube->floor_color = 0x8B4513;
	}
	printf("Floor color: %d\n", cube->floor_color);
	(*i)++;
}

void	process_ceiling_color(char *line, int *j, t_cube *cube, int *i)
{
	if (cube->ceiling_color != -1)
	{
		ft_putstr_fd(2, "Error: Duplicate C identifier\n");
		exit(EXIT_FAILURE);
	}
	(*j)++;
	while (line[*j] == ' ')
		(*j)++;
	cube->ceiling_color = rgb_to_int(line + *j);
	if (cube->ceiling_color == -1)
	{
		ft_putstr_fd(2, "Error: Invalid ceiling color value. Using default Sky Blue.\n");
		cube->ceiling_color = 0x87CEEB;
	}
	printf("Ceiling color: %d\n", cube->ceiling_color);
	(*i)++;
}

void	parse_texture_identifier(char *line, char **textures,
		t_cube *cube, int *vars)
{
	while (line[vars[1]] == ' ')
		vars[1]++;
	if (!ft_strncmp(&line[vars[1]], "NO ", 3)
		|| !ft_strncmp(&line[vars[1]], "SO ", 3)
		|| !ft_strncmp(&line[vars[1]], "WE ", 3)
		|| !ft_strncmp(&line[vars[1]], "EA ", 3))
		process_texture_wall(line, textures, vars);
	else if (!ft_strncmp(&line[vars[1]], "F ", 2))
		process_floor_color(line, &vars[1], cube, &vars[0]);
	else if (!ft_strncmp(&line[vars[1]], "C ", 2))
		process_ceiling_color(line, &vars[1], cube, &vars[0]);
	else
	{
		ft_putstr_fd(2, "Unknown texture identifier\n");
		exit(EXIT_FAILURE);
	}
}

void	read_texture_lines(int fd, char **textures, t_cube *cube, int *vars)
{
	char	*line;

	line = get_next_line(fd);
	while (vars[0] < 6 && line)
	{
		while (line && (line[0] == '\n' || line[0] == '\0'))
		{
			free(line);
			line = get_next_line(fd);
		}
		if (!line)
			break ;
		parse_texture_identifier(line, textures, cube, vars);
		free(line);
		line = get_next_line(fd);
		vars[1] = 0;
	}
	if (line)
		free(line);
}
