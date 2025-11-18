/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 16:42:11 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/18 13:01:04 by tcarlier         ###   ########.fr       */
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
}

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

int	update_game_state(t_cube *cube)
{
	t_gs	gs;

	if (cube->hook.key_pressed[KEY_SHIFT])
		cube->move_speed = WALK_SPEED + SPRINT_SPEED;
	else
		cube->move_speed = WALK_SPEED;
	if (cube->hook.key_pressed[KEY_ESC])
	{
		ft_mlx_loop_end(cube);
		return (0);
	}
	hooking(&gs, cube);
	if (!cube->img.img)
	{
		cube->img.img = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
		cube->img.addr = mlx_get_data_addr(cube->img.img,
				&cube->img.bits_per_pixel, &cube->img.line_length,
				&cube->img.endian);
	}
	raycast(cube);
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img.img, 0, 0);
	return (0);
}
