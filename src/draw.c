/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:40:11 by tcarlier          #+#    #+#             */
/*   Updated: 2025/10/30 19:47:34 by tcarlier         ###   ########.fr       */
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
	c.r = atoi(rgb + i);
	while (rgb[i] >= '0' && rgb[i] <= '9')
		i++;
	i++;
	c.g = atoi(rgb + i);
	while (rgb[i] >= '0' && rgb[i] <= '9')
		i++;
	i++;
	c.b = atoi(rgb + i);
	if (c.r < 0 || c.r > 255 || c.g < 0 || c.g > 255 || c.b < 0 || c.b > 255)
	{
		fprintf(stderr, "Invalid RGB value: %s\n", rgb);
		exit(EXIT_FAILURE);
	}
	return (c.r << 16 | c.g << 8 | c.b);
}

void	draw_texture(t_cube *cube, int x, int y, t_img texture)
{
	int				i;
	int				j;
	unsigned int	color;

	j = 0;
	while (j < texture.height)
	{
		i = 0;
		while (i < texture.width)
		{
			color = *(unsigned int *)(texture.addr + (j * texture.line_length
						+ i * (texture.bits_per_pixel / 8)));
			my_mlx_pixel_put(&cube->img, x + i, y + j, color);
			i++;
		}
		j++;
	}
}

//en gros la minimap est dessinee bloc par bloc et 
//pas pixel par pixel donc c'est beaucoup plus opti
void	draw_minimap(t_cube *cube)
{
	int minimap_width = 9 * MINIMAP_SIZE;
	int minimap_height = 9 * MINIMAP_SIZE;
	int offset_x = 50;
	int offset_y = HEIGHT - minimap_height - 50;

	for (int j = 0; j <= 9; j++)
	{
		int map_y = (int)cube->player_y - 4 + j;
		if (map_y < 0 || map_y >= cube->map_height)
			continue;
		for (int i = 0; i <= 9; i++)
		{
			int map_x = (int)cube->player_x - 4 + i;
			if (map_x < 0 || map_x >= cube->map_width)
				continue;
			int color = color_map(cube->map[map_y][map_x]);
			int base_x = offset_x + i * MINIMAP_SIZE;
			int base_y = offset_y + j * MINIMAP_SIZE;

			if (MINIMAP_SIZE == 1)
			{
				my_mlx_pixel_put(&cube->img, base_x, base_y, color);
			}
			else
			{
				for (int y = 0; y < MINIMAP_SIZE; y++)
				{
					char *dst = cube->img.addr + ((base_y + y) * cube->img.line_length + base_x * (cube->img.bits_per_pixel / 8));
					for (int x = 0; x < MINIMAP_SIZE; x++)
					{
						*(unsigned int*)dst = color;
						dst += (cube->img.bits_per_pixel / 8);
					}
				}
			}
		}
	}
	int px = offset_x + 4 * MINIMAP_SIZE + MINIMAP_SIZE / 2;
	int py = offset_y + 4 * MINIMAP_SIZE + MINIMAP_SIZE / 2;
	for (int y = -2; y <= 2; y++)
	{
		for (int x = -2; x <= 2; x++)
		{
			my_mlx_pixel_put(&cube->img, px + x, py + y, 0xFF0000);
		}
	}
	int dx = (int)(cube->dir_x * MINIMAP_SIZE * 2.5);
	int dy = (int)(cube->dir_y * MINIMAP_SIZE * 2.5);
	for (int y = -1; y <= 1; y++)
	{
		for (int x = -1; x <= 1; x++)
		{
			my_mlx_pixel_put(&cube->img, px + dx/2 + x, py + dy/2 + y, 0x00FF00);
		}
	}
}
