/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 06:00:00 by igilbert          #+#    #+#             */
/*   Updated: 2025/11/17 06:18:13 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	init_minimap_vars(int *vars)
{
	vars[0] = 9 * MINIMAP_SIZE;
	vars[1] = 9 * MINIMAP_SIZE;
	vars[2] = 50;
	vars[3] = HEIGHT - vars[1] - 50;
}

void	draw_minimap_pixel(t_cube *cube, int base_x, int base_y, int color)
{
	int		x;
	int		y;
	char	*dst;

	y = 0;
	while (y < MINIMAP_SIZE)
	{
		dst = cube->img.addr + ((base_y + y) * cube->img.line_length
				+ base_x * (cube->img.bits_per_pixel / 8));
		x = 0;
		while (x < MINIMAP_SIZE)
		{
			*(unsigned int *)dst = color;
			dst += (cube->img.bits_per_pixel / 8);
			x++;
		}
		y++;
	}
}

void	process_minimap_cell(t_cube *cube, int *vars, int *map_vars, int i)
{
	map_vars[0] = (int)cube->player_x - 4 + i;
	if (map_vars[0] < 0 || map_vars[0] >= cube->map_width)
		return ;
	map_vars[2] = color_map(cube->map[map_vars[1]][map_vars[0]]);
	map_vars[3] = vars[2] + i * MINIMAP_SIZE;
	if (MINIMAP_SIZE == 1)
		my_mlx_pixel_put(&cube->img, map_vars[3], map_vars[4], map_vars[2]);
	else
		draw_minimap_pixel(cube, map_vars[3], map_vars[4], map_vars[2]);
}

void	draw_minimap_grid(t_cube *cube, int *vars)
{
	int	i;
	int	j;
	int	map_vars[6];

	j = 0;
	while (j <= 9)
	{
		map_vars[1] = (int)cube->player_y - 4 + j;
		map_vars[4] = vars[3] + j * MINIMAP_SIZE;
		j++;
		if (map_vars[1] < 0 || map_vars[1] >= cube->map_height)
			continue ;
		i = 0;
		while (i <= 9)
		{
			process_minimap_cell(cube, vars, map_vars, i);
			i++;
		}
	}
}

void	draw_minimap_direction(t_cube *cube, int px, int py, int *dir_vars)
{
	int	i;
	int	j;

	j = -1;
	while (j <= 1)
	{
		i = -1;
		while (i <= 1)
		{
			my_mlx_pixel_put(&cube->img, px + dir_vars[0] / 2 + i,
				py + dir_vars[1] / 2 + j, 0x00FF00);
			i++;
		}
		j++;
	}
}
