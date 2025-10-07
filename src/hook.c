/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:42:11 by tcarlier          #+#    #+#             */
/*   Updated: 2025/10/07 17:46:52 by tcarlier         ###   ########.fr       */
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
	if (keycode == 14)
		cube->hook.key_pressed[KEY_E] = true;
	if (keycode == 257)
		cube->hook.key_pressed[KEY_SHIFT] = !cube->hook.key_pressed[KEY_SHIFT];
	
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
	int	k;

	k = 0;
	
	if (cube->hook.key_pressed[KEY_SHIFT])
	{
		cube->move_speed =  WALK_SPEED + SPRINT_SPEED;
	}
	else
	{
		cube->move_speed = WALK_SPEED;
	}
	if (cube->hook.key_pressed[KEY_ESC])
	{
		ft_mlx_loop_end(cube);
		return (0);
	}
	if (cube->hook.key_pressed[KEY_LEFT] || cube->hook.mouse_pos[0])
	{
		while (k < SENSI)
		{
			double old_dir_x = cube->dir_x;
			cube->dir_x = cube->dir_x * cos(-LA_ROTA) - cube->dir_y * sin(-LA_ROTA);
			cube->dir_y = old_dir_x * sin(-LA_ROTA) + cube->dir_y * cos(-LA_ROTA);
			double old_plane_x = cube->plane_x;
			cube->plane_x = cube->plane_x * cos(-LA_ROTA) - cube->plane_y * sin(-LA_ROTA);
			cube->plane_y = old_plane_x * sin(-LA_ROTA) + cube->plane_y * cos(-LA_ROTA);
			k++;
		}
	}
	k = 0;
	if (cube->hook.key_pressed[KEY_RIGHT] || cube->hook.mouse_pos[1])
	{
		while (k < SENSI)
		{
			double old_dir_x = cube->dir_x;
			cube->dir_x = cube->dir_x * cos(LA_ROTA) - cube->dir_y * sin(LA_ROTA);
			cube->dir_y = old_dir_x * sin(LA_ROTA) + cube->dir_y * cos(LA_ROTA);
			double old_plane_x = cube->plane_x;
			cube->plane_x = cube->plane_x * cos(LA_ROTA) - cube->plane_y * sin(LA_ROTA);
			cube->plane_y = old_plane_x * sin(LA_ROTA) + cube->plane_y * cos(LA_ROTA);
			k++;
		}
	}
	if (cube->hook.key_pressed[KEY_W])
	{
		double new_x = cube->player_x + (cube->dir_x * cube->move_speed);
		double new_y = cube->player_y + (cube->dir_y * cube->move_speed);

		bool can_move_x = (cube->map[(int)(cube->player_y)][(int)(new_x + (new_x > cube->player_x ? MARGE_COLLISION : -MARGE_COLLISION))] == '0') || (cube->map[(int)(cube->player_y)][(int)(new_x + (new_x > cube->player_x ? MARGE_COLLISION : -MARGE_COLLISION))] == '3');
		bool can_move_y = (cube->map[(int)(new_y + (new_y > cube->player_y ? MARGE_COLLISION : -MARGE_COLLISION))] != NULL &&
						   (cube->map[(int)(new_y + (new_y > cube->player_y ? MARGE_COLLISION : -MARGE_COLLISION))][(int)(cube->player_x)] == '0')) || (cube->map[(int)(new_y + (new_y > cube->player_y ? MARGE_COLLISION : -MARGE_COLLISION))][(int)(cube->player_x)] == '3');
		if (can_move_x)
			cube->player_x += (cube->dir_x * cube->move_speed);
		if (can_move_y)
			cube->player_y += (cube->dir_y * cube->move_speed);
    }
	if (cube->hook.key_pressed[KEY_A])
	{
		double new_x = cube->player_x - (cube->plane_x * cube->move_speed);
		double new_y = cube->player_y - (cube->plane_y * cube->move_speed);
		
		bool can_move_x = (cube->map[(int)(cube->player_y)][(int)(new_x + (new_x < cube->player_x ? -MARGE_COLLISION : MARGE_COLLISION))] == '0') || (cube->map[(int)(cube->player_y)][(int)(new_x + (new_x < cube->player_x ? -MARGE_COLLISION : MARGE_COLLISION))] == '3');
		bool can_move_y = (cube->map[(int)(new_y + (new_y < cube->player_y ? -MARGE_COLLISION : MARGE_COLLISION))] != NULL && 
						   (cube->map[(int)(new_y + (new_y < cube->player_y ? -MARGE_COLLISION : MARGE_COLLISION))][(int)(cube->player_x)] == '0')) || (cube->map[(int)(new_y + (new_y < cube->player_y ? -MARGE_COLLISION : MARGE_COLLISION))][(int)(cube->player_x)] == '3');
		if (can_move_x)
			cube->player_x -= (cube->plane_x * cube->move_speed);
		if (can_move_y)
			cube->player_y -= (cube->plane_y * cube->move_speed);
	}
	if (cube->hook.key_pressed[KEY_D])
	{
		double new_x = cube->player_x + (cube->plane_x * cube->move_speed);
		double new_y = cube->player_y + (cube->plane_y * cube->move_speed);
		
		bool can_move_x = (cube->map[(int)(cube->player_y)][(int)(new_x + (new_x > cube->player_x ? MARGE_COLLISION : -MARGE_COLLISION))] == '0') || (cube->map[(int)(cube->player_y)][(int)(new_x + (new_x > cube->player_x ? MARGE_COLLISION : -MARGE_COLLISION))] == '3');
		bool can_move_y = (cube->map[(int)(new_y + (new_y > cube->player_y ? MARGE_COLLISION : -MARGE_COLLISION))] != NULL &&
						   (cube->map[(int)(new_y + (new_y > cube->player_y ? MARGE_COLLISION : -MARGE_COLLISION))][(int)(cube->player_x)] == '0' || cube->map[(int)(new_y + (new_y > cube->player_y ? MARGE_COLLISION : -MARGE_COLLISION))][(int)(cube->player_x)] == '3'));
		if (can_move_x)
			cube->player_x += (cube->plane_x * cube->move_speed);
		if (can_move_y)
			cube->player_y += (cube->plane_y * cube->move_speed);
	}
	if (cube->hook.key_pressed[KEY_S])
	{
		double new_x = cube->player_x - (cube->dir_x * cube->move_speed);
		double new_y = cube->player_y - (cube->dir_y * cube->move_speed);

		bool can_move_x = (cube->map[(int)(cube->player_y)][(int)(new_x + (new_x < cube->player_x ? -MARGE_COLLISION : MARGE_COLLISION))] == '0' || cube->map[(int)(cube->player_y)][(int)(new_x + (new_x < cube->player_x ? -MARGE_COLLISION : MARGE_COLLISION))] == '3');
		bool can_move_y = (cube->map[(int)(new_y + (new_y < cube->player_y ? -MARGE_COLLISION : MARGE_COLLISION))] != NULL && 
						   (cube->map[(int)(new_y + (new_y < cube->player_y ? -MARGE_COLLISION : MARGE_COLLISION))][(int)(cube->player_x)] == '0' || cube->map[(int)(new_y + (new_y < cube->player_y ? -MARGE_COLLISION : MARGE_COLLISION))][(int)(cube->player_x)] == '3'));
		if (can_move_x)
			cube->player_x -= (cube->dir_x * cube->move_speed);
		if (can_move_y)
			cube->player_y -= (cube->dir_y * cube->move_speed);
	}
	if (cube->hook.key_pressed[KEY_E])
	{
		int front_x = (int)(cube->player_x + cube->dir_x);
		int front_y = (int)(cube->player_y + cube->dir_y);
		if (cube->map[front_y][front_x] == '2')
		{
			cube->map[front_y][front_x] = '3';
			cube->hook.key_pressed[KEY_E] = false;
		}
		else if (cube->map[front_y][front_x] == '3')
		{
			cube->map[front_y][front_x] = '2';
			cube->hook.key_pressed[KEY_E] = false;
		}
	}
		mlx_destroy_image(cube->mlx, cube->img.img);
		cube->img.img = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
		cube->img.addr = mlx_get_data_addr(cube->img.img,
				&cube->img.bits_per_pixel, &cube->img.line_length,
				&cube->img.endian);
		raycast(cube);
		mlx_put_image_to_window(cube->mlx, cube->win, cube->img.img, 0, 0);
	return (0);
}

int key_release_hook(int keycode, t_cube *cube)
{
	if (keycode == 123)
		cube->hook.key_pressed[KEY_LEFT] = false;
	if (keycode == 124)
		cube->hook.key_pressed[KEY_RIGHT] = false;
	if (keycode == 13)
		cube->hook.key_pressed[KEY_W] = false;
	if (keycode == 0)
		cube->hook.key_pressed[KEY_A] = false;
	if (keycode == 2)
		cube->hook.key_pressed[KEY_D] = false;
	if (keycode == 1)
		cube->hook.key_pressed[KEY_S] = false;
	if (keycode == 14)
		cube->hook.key_pressed[KEY_E] = false;
	return (0);
}
