/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:34:41 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/20 15:55:52 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	do_key_d(t_gs gs, t_cube *cube)
{
	gs.new_x = cube->player_x + (cube->plane_x * cube->move_speed);
	gs.new_y = cube->player_y + (cube->plane_y * cube->move_speed);
	if (can_move_x(cube, gs))
		cube->player_x += (cube->plane_x * cube->move_speed);
	if (can_move_y(cube, gs))
		cube->player_y += (cube->plane_y * cube->move_speed);
}

void	do_key_e(t_gs gs, t_cube *cube)
{
	gs.front_x = (int)(cube->player_x + cube->dir_x);
	gs.front_y = (int)(cube->player_y + cube->dir_y);
	if (cube->map[gs.front_y][gs.front_x] == '2')
	{
		cube->map[gs.front_y][gs.front_x] = '3';
		cube->hook.key_pressed[KEY_E] = false;
	}
	else if (cube->map[gs.front_y][gs.front_x] == '3')
	{
		cube->map[gs.front_y][gs.front_x] = '2';
		cube->hook.key_pressed[KEY_E] = false;
	}
}

void	hooking(t_gs *gs, t_cube *cube)
{
	if (cube->hook.key_pressed[KEY_LEFT])
		do_key_left(*gs, cube);
	if (cube->hook.key_pressed[KEY_RIGHT])
		do_key_right(*gs, cube);
	if (cube->hook.key_pressed[KEY_W])
		do_key_w(*gs, cube);
	if (cube->hook.key_pressed[KEY_A])
		do_key_a(*gs, cube);
	if (cube->hook.key_pressed[KEY_S])
		do_key_s(*gs, cube);
	if (cube->hook.key_pressed[KEY_D])
		do_key_d(*gs, cube);
	if (cube->hook.key_pressed[KEY_E])
		do_key_e(*gs, cube);
}

//linux
/*
int	key_release_hook(int keycode, t_cube *cube)
{
	if (keycode == 65361)
		cube->hook.key_pressed[KEY_LEFT] = false;
	if (keycode == 65363)
		cube->hook.key_pressed[KEY_RIGHT] = false;
	if (keycode == 119)
		cube->hook.key_pressed[KEY_W] = false;
	if (keycode == 97)
		cube->hook.key_pressed[KEY_A] = false;
	if (keycode == 100)
		cube->hook.key_pressed[KEY_D] = false;
	if (keycode == 115)
		cube->hook.key_pressed[KEY_S] = false;
	if (keycode == 101)
		cube->hook.key_pressed[KEY_E] = false;
	return (0);
}*/
