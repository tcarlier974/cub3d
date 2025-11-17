/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:40:11 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/17 06:43:13 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	rgb_to_int(const char *rgb)
{
	t_color	c;
	int		i;

	i = 0;
	while (rgb[i] == ' ')
		i++;
	c.r = ft_atoi(rgb + i);
	while (rgb[i] >= '0' && rgb[i] <= '9')
		i++;
	i++;
	c.g = ft_atoi(rgb + i);
	while (rgb[i] >= '0' && rgb[i] <= '9')
		i++;
	i++;
	c.b = ft_atoi(rgb + i);
	if (c.r < 0 || c.r > 255 || c.g < 0 || c.g > 255 || c.b < 0 || c.b > 255)
	{
		write(2, "Invalid RGB value\n", 19);
		exit(EXIT_FAILURE);
	}
	return (c.r << 16 | c.g << 8 | c.b);
}

void	draw_minimap(t_cube *cube)
{
	int	vars[6];

	init_minimap_vars(vars);
	draw_minimap_grid(cube, vars);
	draw_minimap_player(cube, vars);
}

void	draw_minimap_player(t_cube *cube, int *vars)
{
	int	i;
	int	j;
	int	px;
	int	py;
	int	dir_vars[2];

	px = vars[2] + 4 * MINIMAP_SIZE + MINIMAP_SIZE / 2;
	py = vars[3] + 4 * MINIMAP_SIZE + MINIMAP_SIZE / 2;
	j = -2;
	while (j <= 2)
	{
		i = -2;
		while (i <= 2)
		{
			my_mlx_pixel_put(&cube->img, px + i, py + j, 0xFF0000);
			i++;
		}
		j++;
	}
	dir_vars[0] = (int)(cube->dir_x * MINIMAP_SIZE * 2.5);
	dir_vars[1] = (int)(cube->dir_y * MINIMAP_SIZE * 2.5);
	draw_minimap_direction(cube, px, py, dir_vars);
}
