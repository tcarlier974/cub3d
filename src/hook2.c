/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 22:47:38 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/02 22:58:54 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	do_key_left(t_gs gs, t_cube *cube)
{
	gs.old_dir_x = cube->dir_x;
	cube->dir_x = cube->dir_x * cos(-ROTA) - cube->dir_y * sin(-ROTA);
	cube->dir_y = gs.old_dir_x * sin(-ROTA) + cube->dir_y * cos(-ROTA);
	gs.old_plane_x = cube->plane_x;
	cube->plane_x = cube->plane_x * cos(-ROTA) - cube->plane_y * sin(-ROTA);
	cube->plane_y = gs.old_plane_x * sin(-ROTA) + cube->plane_y * cos(-ROTA);
}

void	do_key_right(t_gs gs, t_cube *cube)
{
	gs.old_dir_x = cube->dir_x;
	cube->dir_x = cube->dir_x * cos(ROTA) - cube->dir_y * sin(ROTA);
	cube->dir_y = gs.old_dir_x * sin(ROTA) + cube->dir_y * cos(ROTA);
	gs.old_plane_x = cube->plane_x;
	cube->plane_x = cube->plane_x * cos(ROTA) - cube->plane_y * sin(ROTA);
	cube->plane_y = gs.old_plane_x * sin(ROTA) + cube->plane_y * cos(ROTA);
}

void	do_key_a(t_gs gs, t_cube *cube)
{
	gs.new_x = cube->player_x - (cube->plane_x * cube->move_speed);
	gs.new_y = cube->player_y - (cube->plane_y * cube->move_speed);
	if (can_move_x(cube, gs))
		cube->player_x -= (cube->plane_x * cube->move_speed);
	if (can_move_y(cube, gs))
		cube->player_y -= (cube->plane_y * cube->move_speed);
}

void	do_key_s(t_gs gs, t_cube *cube)
{
	gs.new_x = cube->player_x - (cube->dir_x * cube->move_speed);
	gs.new_y = cube->player_y - (cube->dir_y * cube->move_speed);
	if (can_move_x(cube, gs))
		cube->player_x -= (cube->dir_x * cube->move_speed);
	if (can_move_y(cube, gs))
		cube->player_y -= (cube->dir_y * cube->move_speed);
}

void	do_key_w(t_gs gs, t_cube *cube)
{
	gs.new_x = cube->player_x + (cube->dir_x * cube->move_speed);
	gs.new_y = cube->player_y + (cube->dir_y * cube->move_speed);
	if (can_move_x(cube, gs))
		cube->player_x += (cube->dir_x * cube->move_speed);
	if (can_move_y(cube, gs))
		cube->player_y += (cube->dir_y * cube->move_speed);
}

void	do_key_d(t_gs gs, t_cube *cube)
{
	gs.new_x = cube->player_x + (cube->plane_x * cube->move_speed);
	gs.new_y = cube->player_y + (cube->plane_y * cube->move_speed);
	if (can_move_x(cube, gs))
		cube->player_x += (cube->plane_x * cube->move_speed);
	if (can_move_y(cube, gs))
		cube->player_y += (cube->plane_y * cube->move_speed);
}
