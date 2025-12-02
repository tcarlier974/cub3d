/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 19:11:59 by tcarlier          #+#    #+#             */
/*   Updated: 2025/12/02 17:52:25 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

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

void	ft_exit(int code, t_cube *cube, char *str)
{
	int	i;

	i = 0;
	while (i < TEXTURE_COUNT && cube->texture[i].img)
	{
		if (cube->texture[i].img)
			mlx_destroy_image(cube->mlx, cube->texture[i].img);
		i++;
	}
	i = 0;
	if (cube->map)
	{
		while (cube->map[i])
			free(cube->map[i++]);
		free(cube->map);
	}
	if (code == 7)
		ft_putstr_fd(2, str);
	else
		(void)str;
	mlx_destroy_window(cube->mlx, cube->win);
	if (cube->img.img)
		mlx_destroy_image(cube->mlx, cube->img.img);
	free(cube->mlx);
	exit(1);
}
