/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:44:29 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/02 22:46:28 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	second_init_ray(t_raycast *ray, t_cube *cube, int x)
{
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (cube->player_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - cube->player_y)
			* ray->delta_dist_y;
	}
}

void	init_raycast_values(t_raycast *r, t_cube *cube, int x)
{
	r->hit = 0;
	r->map_x = (int)cube->player_x;
	r->map_y = (int)cube->player_y;
	r->camera_x = 2 * (double)x / (double)WIDTH - 1;
	r->ray_dir_x = cube->dir_x + cube->plane_x * r->camera_x;
	r->ray_dir_y = cube->dir_y + cube->plane_y * r->camera_x;
	if (r->ray_dir_x == 0)
		r->delta_dist_x = 1e30;
	else
		r->delta_dist_x = fabs(1 / r->ray_dir_x);
	if (r->ray_dir_y == 0)
		r->delta_dist_y = 1e30;
	else
		r->delta_dist_y = fabs(1 / r->ray_dir_y);
	if (r->ray_dir_x < 0)
	{
		r->step_x = -1;
		r->side_dist_x = (cube->player_x - r->map_x) * r->delta_dist_x;
	}
	else
	{
		r->step_x = 1;
		r->side_dist_x = (r->map_x + 1.0 - cube->player_x) * r->delta_dist_x;
	}
	second_init_ray(r, cube, x);
}

void	dda_algo(t_raycast *ray, t_cube *cube)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
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
}

int	color_map(char c)
{
	if (c == '1')
		return (0x222222);
	else if (c == '0')
		return (0xD3D3D3);
	else
		return (0x808080);
}

void	raycast(t_cube *cube)
{
	t_raycast	ray;
	t_algo		var;

	var.x = 0;
	var.tex_num = 0;
	while (var.x < WIDTH)
	{
		init_raycast_values(&ray, cube, var.x);
		dda_algo(&ray, cube);
		what_to_draw(cube, &var, ray);
		while (var.y < ray.draw_start)
		{
			my_mlx_pixel_put(&cube->img, var.x, var.y, cube->ceiling_color);
			var.y++;
		}
		draw_col(cube, &var, ray);
		while (var.y < HEIGHT)
		{
			my_mlx_pixel_put(&cube->img, var.x, var.y, cube->floor_color);
			var.y++;
		}
		var.x++;
	}
	if (ENABLE_MINIMAP)
		draw_minimap(cube);
}
