/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:42:11 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/30 19:12:15 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

int	ft_mlx_loop_end(t_cube *cube)
{
	int	i;

	i = 0;
	mlx_destroy_window(cube->mlx, cube->win);
	mlx_destroy_image(cube->mlx, cube->img.img);
	while (i < TEXTURE_COUNT)
	{
		if (cube->texture[i].img)
			mlx_destroy_image(cube->mlx, cube->texture[i].img);
		i++;
	}
	i = 0;
	while (cube->map[i])
	{
		free(cube->map[i]);
		i++;
	}
	free(cube->map);
	exit(0);
	return (0);
}
//linux
/*
int	key_hook_press(int keycode, t_cube *cube)
{
	if (keycode == 65307)
		cube->hook.key_pressed[KEY_ESC] = true;
	if (keycode == 65361)
		cube->hook.key_pressed[KEY_LEFT] = true;
	if (keycode == 65363)
		cube->hook.key_pressed[KEY_RIGHT] = true;
	if (keycode == 119)
		cube->hook.key_pressed[KEY_W] = true;
	if (keycode == 97)
		cube->hook.key_pressed[KEY_A] = true;
	if (keycode == 100)
		cube->hook.key_pressed[KEY_D] = true;
	if (keycode == 115)
		cube->hook.key_pressed[KEY_S] = true;
	if (keycode == 101)
		cube->hook.key_pressed[KEY_E] = true;
	if (keycode == 65505)
		cube->hook.key_pressed[KEY_SHIFT] = !cube->hook.key_pressed[KEY_SHIFT];
	return (0);
}*/
//macos

int	key_release_hook(int keycode, t_cube *cube)
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

int	key_hook_press(int keycode, t_cube *cube)
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
	return (0);
}

//end macos

bool	can_move_x(t_cube *cube, t_gs gs)
{
	if (gs.new_x > cube->player_x)
		gs.new_x += MARGE_COLLISION;
	else
		gs.new_x -= MARGE_COLLISION;
	return ((cube->map[(int)(cube->player_y)][(int)(gs.new_x)] == '0')
		|| (cube->map[(int)(cube->player_y)][(int)(gs.new_x)] == '3'));
}

bool	can_move_y(t_cube *cube, t_gs gs)
{
	if (gs.new_y > cube->player_y)
		gs.new_y += MARGE_COLLISION;
	else
		gs.new_y -= MARGE_COLLISION;
	return (((cube->map[(int)(gs.new_y)][(int)(cube->player_x)] &&
		(cube->map[(int)(gs.new_y)][(int)(cube->player_x)] == '0'))
		|| (cube->map[(int)(gs.new_y)][(int)(cube->player_x)] == '3')));
}
