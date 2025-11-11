/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:16:45 by igilbert          #+#    #+#             */
/*   Updated: 2025/11/11 16:43:56 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	chose_tex(t_raycast ray, t_algo *var, t_cube *cube)
{
	if (ray.side == 0)
	{
		if (ray.ray_dir_x < 0)
			var->tex_num = 3;
		else
			var->tex_num = 2;
	}
	else
	{
		if (ray.ray_dir_y < 0)
			var->tex_num = 0;
		else
			var->tex_num = 1;
	}
	if (cube->map[ray.map_y][ray.map_x] == '2')
		var->tex_num = 4;
}

void	draw_col_conditions(t_cube *cube, t_algo *var, t_raycast ray)
{
	if (ray.side == 0)
		var->wall_x = cube->player_y + ray.perp_wall_dist
			* ray.ray_dir_y;
	else
		var->wall_x = cube->player_x + ray.perp_wall_dist
			* ray.ray_dir_x;
	var->wall_x -= floor(var->wall_x);
	var->tex_x = (int)(var->wall_x * (double)var->tex_width);
	if (var->tex_x < 0)
		var->tex_x = 0;
	if (var->tex_x >= var->tex_width)
		var->tex_x = var->tex_width - 1;
	if ((ray.side == 0 && ray.ray_dir_x < 0)
		|| (ray.side == 1 && ray.ray_dir_y > 0))
		var->tex_x = var->tex_width - var->tex_x - 1;
	if (var->tex_y < 0)
		var->tex_y = 0;
	if (var->tex_y >= var->tex_height)
		var->tex_y = var->tex_height - 1;
	var->color = *(unsigned int *)(cube->texture[var->tex_num].addr
			+ (var->tex_y
				* cube->texture[var->tex_num].line_length + var->tex_x
				* (cube->texture[var->tex_num].bits_per_pixel / 8)));
}

void	draw_col(t_cube *cube, t_algo *var, t_raycast ray)
{
	while (var->y < ray.draw_end)
	{
		if (cube->map[ray.map_y][ray.map_x] != '0')
		{
			chose_tex(ray, var, cube);
			var->tex_height = cube->texture[var->tex_num].height;
			var->tex_width = cube->texture[var->tex_num].width;
			var->d = (var->y * 2 - HEIGHT + ray.line_height) * 128;
			var->tex_y = ((var->d * var->tex_height) / ray.line_height) / 256;
			draw_col_conditions(cube, var, ray);
		}
		my_mlx_pixel_put(&cube->img, var->x, var->y, var->color);
		var->y++;
	}
}

void	what_to_draw(t_cube *cube, t_algo *var, t_raycast *ray)
{
	if (cube->map[ray->map_y][ray->map_x] != '0'
			&& cube->map[ray->map_y][ray->map_x] != '3')
	{
		ray->line_height = (int)(HEIGHT / ray->perp_wall_dist);
		ray->draw_start = (-ray->line_height / 2) + (HEIGHT / 2);
		if (ray->draw_start < 0)
			ray->draw_start = 0;
		ray->draw_end = (ray->line_height / 2) + (HEIGHT / 2);
		if (ray->draw_end >= HEIGHT)
			ray->draw_end = HEIGHT - 1;
		var->y = 0;
	}
}

void	dda_algo_conditions(t_cube *cube, t_raycast *ray)
{
	if (cube->map[ray->map_y][ray->map_x] == '1')
	{
		ray->hit = 1;
		if (ray->side == 0)
			ray->perp_wall_dist = (ray->side_dist_x - ray->delta_dist_x);
		else
			ray->perp_wall_dist = (ray->side_dist_y - ray->delta_dist_y);
	}
	else if (cube->map[ray->map_y][ray->map_x] == '2')
	{
		ray->hit = 2;
		if (ray->side == 0)
			ray->perp_wall_dist = (ray->side_dist_x - ray->delta_dist_x);
		else
			ray->perp_wall_dist = (ray->side_dist_y - ray->delta_dist_y);
	}
}
