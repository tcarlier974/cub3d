/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:28:08 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/20 17:10:33 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

char	*process_texture_line(char *line, int *j, int idx);

void	process_texture_wall(char *line, char **textures, int *vars)
{
	if (line[vars[1]] == 'N')
	{
		if (textures[0])
		{
			ft_putstr_fd(2, "Error: Duplicate NO texture identifier\n");
			exit(EXIT_FAILURE);
		}
		textures[0] = process_texture_line(line, &vars[1], 0);
		vars[0]++;
	}
	else if (line[vars[1]] == 'S')
	{
		if (textures[1])
		{
			ft_putstr_fd(2, "Error: Duplicate SO texture identifier\n");
			exit(EXIT_FAILURE);
		}
		textures[1] = process_texture_line(line, &vars[1], 1);
		vars[0]++;
	}
	else if (line[vars[1]] == 'E')
	{
		if (textures[2])
		{
			ft_putstr_fd(2, "Error: Duplicate EA texture identifier\n");
			exit(EXIT_FAILURE);
		}
		textures[2] = process_texture_line(line, &vars[1], 2);
		vars[0]++;
	}
	else if (line[vars[1]] == 'W')
	{
		if (textures[3])
		{
			ft_putstr_fd(2, "Error: Duplicate WE texture identifier\n");
			exit(EXIT_FAILURE);
		}
		textures[3] = process_texture_line(line, &vars[1], 3);
		vars[0]++;
	}
}

void	texture_recup(t_cube *cube, char ***textures, int i)
{
	if (!(*textures)[i])
	{
		ft_putstr_fd(2, "Texture path not found\n");
		exit(EXIT_FAILURE);
	}
	cube->texture[i].img = NULL;
	cube->texture[i].bits_per_pixel = 0;
	cube->texture[i].line_length = 0;
	cube->texture[i].endian = 0;
	cube->texture[i].width = 0;
	cube->texture[i].height = 0;
	cube->texture[i].img = mlx_xpm_file_to_image(cube->mlx, (*textures)[i],
			&cube->texture[i].width, &cube->texture[i].height);
	if (!cube->texture[i].img)
	{
		ft_putstr_fd(2, "Failed to load texture\n");
		exit(EXIT_FAILURE);
	}
	free((*textures)[i]);
	cube->texture[i].addr = mlx_get_data_addr(cube->texture[i].img,
			&cube->texture[i].bits_per_pixel, &cube->texture[i].line_length,
			&cube->texture[i].endian);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	size_t		i;
	long long	res;
	long long	signe;

	i = 0;
	signe = 1;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		signe = -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (res * signe);
}
