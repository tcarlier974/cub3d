/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 19:11:59 by tcarlier          #+#    #+#             */
/*   Updated: 2025/12/01 02:04:38 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

char	*fps_calculator(int fps)
{
	static struct timeval	last_time = {0, 0};
	static int				frame_count = 0;
	static char				fps_str[20];
	struct timeval			current_time;
	double					elapsed;

	gettimeofday(&current_time, NULL);
	frame_count++;
	elapsed = (current_time.tv_sec - last_time.tv_sec)
		+ (current_time.tv_usec - last_time.tv_usec) / 1000000.0;
	if (elapsed >= 1.0)
	{
		fps = (int)(frame_count / elapsed);
		snprintf(fps_str, sizeof(fps_str), "FPS: %d", fps);
		last_time = current_time;
		frame_count = 0;
	}
	return (fps_str);
}

int	update_game_state(t_cube *cube)
{
	t_gs	gs;
	int		fps;

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
	mlx_string_put(cube->mlx, cube->win, 10, 10, 0x00FF00, fps_calculator(fps));
	return (0);
}
