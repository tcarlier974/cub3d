/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:40:11 by tcarlier          #+#    #+#             */
/*   Updated: 2025/10/24 19:31:38 by tcarlier         ###   ########.fr       */
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
	int r, g, b;
	int i = 0;
	while (rgb[i] == ' ')
		i++;
	r = atoi(rgb + i);
	while (rgb[i] >= '0' && rgb[i] <= '9')
		i++;
	i++;
	g = atoi(rgb + i);
	while (rgb[i] >= '0' && rgb[i] <= '9')
		i++;
	i++;
	b = atoi(rgb + i);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
	{
		fprintf(stderr, "Invalid RGB value: %s\n", rgb);
		exit(EXIT_FAILURE);
	}
	write(1, "RGB values: ", 12);
	printf("R: %d, G: %d, B: %d\n", r, g, b);
	return (r << 16 | g << 8 | b);
}

void	draw_texture(t_cube *cube, int x, int y, t_img texture)
{
	int		i;
	int		j;
	unsigned int	color;

	for (j = 0; j < texture.height; j++)
	{
		for (i = 0; i < texture.width; i++)
		{
			color = *(unsigned int *)(texture.addr + (j * texture.line_length + i * (texture.bits_per_pixel / 8)));
			my_mlx_pixel_put(&cube->img, x + i, y + j, color);
		}
	}
}
//en gros la minimap est dessinee bloc par bloc et pas pixel par pixel donc c'est beaucoup plus opti
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

void draw_sprite(t_cube *cube)
{
	if (cube->sprite.img)
	{
		int sprite_width = cube->sprite.width;
		int sprite_height = cube->sprite.height;
		int x = (WIDTH - sprite_width) / 2;
		int y = HEIGHT - sprite_height - 30;
		while (y < HEIGHT - 30 + sprite_height)
		{
			if (y < 0 || y >= HEIGHT)
				break;
			int i = 0;
			while (i < sprite_width)
			{
				if (x + i < 0 || x + i >= WIDTH)
					break;
				unsigned int color = *(unsigned int *)(cube->sprite.addr + (y * cube->sprite.line_length + i * (cube->sprite.bits_per_pixel / 8)));
				if (color != 0x000000)
					my_mlx_pixel_put(&cube->img, x + i, y, color);
				i++;
			}
			y++;
		}
	}
}
