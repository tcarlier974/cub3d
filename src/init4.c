/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 06:20:00 by igilbert          #+#    #+#             */
/*   Updated: 2025/12/02 17:05:16 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

char	*process_texture_line(char *line, int *j, t_cube *cube)
{
	char	*texture;
	char	*newline;

	*j += 2;
	while (line[*j] == ' ' || line[*j] == '\t')
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
		ft_putstr_fd(2, "Error : Failed to allocate memory for texture path\n");
		ft_exit(1, cube, NULL);
	}
	return (texture);
}

void	process_floor_color(char *line, int *j, t_cube *cube, int *i)
{
	if (cube->floor_color != -1)
	{
		ft_putstr_fd(2, "Error : Duplicate F identifier\n");
		ft_exit(1, cube, NULL);
	}
	(*j)++;
	while (line[*j] == ' ' || line[*j] == '\t')
		(*j)++;
	cube->floor_color = rgb_to_int(line + *j);
	if (cube->floor_color == -1)
	{
		ft_putstr_fd(2, "Error : Invalid floor color. Using default Brown.\n");
		cube->floor_color = 0x8B4513;
	}
	(*i)++;
}

void	process_ceiling_color(char *line, int *j, t_cube *cube, int *i)
{
	if (cube->ceiling_color != -1)
	{
		ft_putstr_fd(2, "Error : Duplicate C identifier\n");
		ft_exit(1, cube, NULL);
	}
	(*j)++;
	while (line[*j] == ' ' || line[*j] == '\t')
		(*j)++;
	cube->ceiling_color = rgb_to_int(line + *j);
	if (cube->ceiling_color == -1)
	{
		ft_putstr_fd(2, "Error : Invalid ceiling color. Using default SkyB.\n");
		cube->ceiling_color = 0x87CEEB;
	}
	(*i)++;
}

void	parse_texture_identifier(char *line, char **textures,
		t_cube *cube, int *vars)
{
	while (line[vars[1]] == ' ' || line[vars[1]] == '\t')
		vars[1]++;
	if ((!ft_strncmp(&line[vars[1]], "NO", 2) && (line[vars[1] + 2] == ' '
				|| line[vars[1] + 2] == '\t'))
		|| (!ft_strncmp(&line[vars[1]], "SO", 2) && (line[vars[1] + 2] == ' '
				|| line[vars[1] + 2] == '\t'))
		|| (!ft_strncmp(&line[vars[1]], "WE", 2) && (line[vars[1] + 2] == ' '
				|| line[vars[1] + 2] == '\t'))
		|| (!ft_strncmp(&line[vars[1]], "EA", 2) && (line[vars[1] + 2] == ' '
				|| line[vars[1] + 2] == '\t')))
		process_texture_wall(line, textures, vars, cube);
	else if (!ft_strncmp(&line[vars[1]], "F", 1) && (line[vars[1] + 1] == ' '
			|| line[vars[1] + 1] == '\t'))
		process_floor_color(line, &vars[1], cube, &vars[0]);
	else if (!ft_strncmp(&line[vars[1]], "C", 1) && (line[vars[1] + 1] == ' '
			|| line[vars[1] + 1] == '\t'))
		process_ceiling_color(line, &vars[1], cube, &vars[0]);
	else
		ft_exit(7, cube, "Error : Invalid texture identifier\n");
}

void	read_texture_lines(int fd, char **textures, t_cube *cube, int *vars)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (vars[0] < 6)
		{
			while (line && (line[0] == '\n' || line[0] == '\0'))
			{
				free(line);
				line = get_next_line(fd);
			}
			if (!line)
				break ;
			parse_texture_identifier(line, textures, cube, vars);
			vars[1] = 0;
		}
		free(line);
		line = get_next_line(fd);
	}
}
