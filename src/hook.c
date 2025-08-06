/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:42:11 by tcarlier          #+#    #+#             */
/*   Updated: 2025/08/06 16:42:54 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

int ft_mlx_loop_end(t_cube *cube)
{
	mlx_destroy_window(cube->mlx, cube->win);
	mlx_destroy_image(cube->mlx, cube->img.img);
	for (int i = 0; i < TEXTURE_COUNT; i++)
	{
		if (cube->texture[i].img)
			mlx_destroy_image(cube->mlx, cube->texture[i].img);
	}
	free(cube->map);
	exit(0);
	return (0);
}

int key_hook_press(int keycode, t_cube *cube)
{
	if (keycode == 53)
		cube->hook.key_pressed[KEY_ESC] = true;
	if (keycode == 123)
		cube->hook.key_pressed[KEY_LEFT] = true;
	if (keycode == 124)
		cube->hook.key_pressed[KEY_RIGHT] = true;
	if (keycode == 13)
		cube->hook.key_pressed[KEY_W] = true;
	if (keycode == 0)
		cube->hook.key_pressed[KEY_A] = true;
	if (keycode == 2)
		cube->hook.key_pressed[KEY_D] = true;
	if (keycode == 1)
		cube->hook.key_pressed[KEY_S] = true;
	if (keycode == 257)
		cube->hook.key_pressed[KEY_SHIFT] = true;
	mlx_mouse_get_pos(cube->mlx, &cube->hook.mouse_x, &cube->hook.mouse_y);
	if (cube->hook.mouse_x < WIDTH / 2)
	{
		cube->hook.mouse_pos[0] = true;
		cube->hook.mouse_pos[1] = false;
	}
	else if (cube->hook.mouse_x > WIDTH / 2)
	{
		cube->hook.mouse_pos[0] = false;
		cube->hook.mouse_pos[1] = true;
	}
	else
	{
		cube->hook.mouse_pos[0] = false;
		cube->hook.mouse_pos[1] = false;
	}
	return (0);
}

int	update_game_state(t_cube *cube)
{
	if (cube->hook.key_pressed[KEY_SHIFT])
		cube->move_speed = SPRINT_SPEED;
	else
		cube->move_speed = WALK_SPEED;
	if (cube->hook.key_pressed[KEY_ESC])
	{
		ft_mlx_loop_end(cube);
		return (0);
	}
	if (cube->hook.key_pressed[KEY_LEFT] || cube->hook.mouse_pos[0])
	{
		double old_dir_x = cube->dir_x;
		cube->dir_x = cube->dir_x * cos(-0.08) - cube->dir_y * sin(-0.08);
		cube->dir_y = old_dir_x * sin(-0.08) + cube->dir_y * cos(-0.08);
		double old_plane_x = cube->plane_x;
		cube->plane_x = cube->plane_x * cos(-0.08) - cube->plane_y * sin(-0.08);
		cube->plane_y = old_plane_x * sin(-0.08) + cube->plane_y * cos(-0.08);

	}
	if (cube->hook.key_pressed[KEY_RIGHT] || cube->hook.mouse_pos[1])
	{
		double old_dir_x = cube->dir_x;
		cube->dir_x = cube->dir_x * cos(0.08) - cube->dir_y * sin(0.08);
		cube->dir_y = old_dir_x * sin(0.08) + cube->dir_y * cos(0.08);
		double old_plane_x = cube->plane_x;
		cube->plane_x = cube->plane_x * cos(0.08) - cube->plane_y * sin(0.08);
		cube->plane_y = old_plane_x * sin(0.08) + cube->plane_y * cos(0.08);
	}
	if (cube->hook.key_pressed[KEY_W])
	{
		if (cube->map[(int)(cube->player_y + (cube->dir_y * cube->move_speed) * (5/3))] != NULL)
		{
			if(cube->map[(int)(cube->player_y + (cube->dir_y * cube->move_speed) * (5/3))][(int)(cube->player_x)] == '0')
				cube->player_y += (cube->dir_y * cube->move_speed);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x + (cube->dir_x * cube->move_speed) * (5/3))] == '0')
			cube->player_x += (cube->dir_x * cube->move_speed);
    }
	if (cube->hook.key_pressed[KEY_A])
	{
		if (cube->map[(int)(cube->player_y - (cube->dir_y * cube->move_speed) * (5/3))] != NULL)
		{
			if(cube->map[(int)(cube->player_y - (cube->plane_y * cube->move_speed) * (5/3))][(int)(cube->player_x)] == '0')
				cube->player_y -= (cube->plane_y * cube->move_speed);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x - (cube->plane_x * cube->move_speed) * (5/3))] == '0')
			cube->player_x -= (cube->plane_x * cube->move_speed);
	}
	if (cube->hook.key_pressed[KEY_D])
	{
		if (cube->map[(int)(cube->player_y + (cube->plane_y * cube->move_speed) * (5/3))] != NULL)
		{
			if(cube->map[(int)(cube->player_y + (cube->plane_y * cube->move_speed) * (5/3))][(int)(cube->player_x)] == '0')
				cube->player_y += (cube->plane_y * cube->move_speed);
		}
		if(cube->map[(int)(cube->player_y)][(int)(cube->player_x + (cube->plane_x * cube->move_speed) * (5/3))] == '0')
			cube->player_x += (cube->plane_x * cube->move_speed);
	}
	if (cube->hook.key_pressed[KEY_S])
	{
		if (cube->map[(int)(cube->player_y - (cube->dir_y * cube->move_speed) * (5/3))])
		{
			if(cube->map[(int)(cube->player_y - (cube->dir_y * cube->move_speed) * (5/3))][(int)(cube->player_x)] == '0')
				cube->player_y -= (cube->dir_y * cube->move_speed);
		}
      	if(cube->map[(int)(cube->player_y)][(int)(cube->player_x - (cube->dir_x * cube->move_speed) * (5/3))] == '0')
	  		cube->player_x -= (cube->dir_x * cube->move_speed);
	}
	if (cube->hook.mouse_pos[0])
	{
		double mouse_drift = (double)(cube->hook.mouse_x + 1);
		if (mouse_drift > WIDTH / 2)
			mouse_drift = WIDTH / 2;
		if (mouse_drift == WIDTH /2)
			cube->hook.mouse_pos[0] = false;
		mlx_mouse_move(cube->mlx, mouse_drift, HEIGHT / 2);
	}
	else if (cube->hook.mouse_pos[1])
	{
		double mouse_drift = (double)(cube->hook.mouse_x - 1);
		if (mouse_drift < WIDTH / 2)
			mouse_drift = WIDTH / 2;
		if (mouse_drift == WIDTH /2)
			cube->hook.mouse_pos[1] = false;
		mlx_mouse_move(cube->mlx, mouse_drift, HEIGHT / 2);
	}
	int i = 0;
	while (i < 6)
	{
		if (cube->hook.key_pressed[i])
			{
				mlx_destroy_image(cube->mlx, cube->img.img);
				cube->img.img = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
				cube->img.addr = mlx_get_data_addr(cube->img.img,
						&cube->img.bits_per_pixel, &cube->img.line_length,
						&cube->img.endian);
				raycast(cube);
				// draw_sprite(cube);
				mlx_put_image_to_window(cube->mlx, cube->win, cube->img.img, 0, 0);
				break;
			}
			i++;
	}
	return (0);
}

int key_release_hook(int keycode, t_hook *hook)
{
	if (keycode == 123)
		hook->key_pressed[KEY_LEFT] = false;
	if (keycode == 124)
		hook->key_pressed[KEY_RIGHT] = false;
	if (keycode == 13)
		hook->key_pressed[KEY_W] = false;
	if (keycode == 0)
		hook->key_pressed[KEY_A] = false;
	if (keycode == 2)
		hook->key_pressed[KEY_D] = false;
	if (keycode == 1)
		hook->key_pressed[KEY_S] = false;
	if (keycode == 257)
		hook->key_pressed[KEY_SHIFT] = false;
	return (0);
}
