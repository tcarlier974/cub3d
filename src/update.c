/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 19:11:59 by tcarlier          #+#    #+#             */
/*   Updated: 2025/12/01 17:27:40 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

int	count_nb(int n)
{
	long int	i;
	int			count;

	count = 0;
	i = n;
	if (n < 0)
	{
		count++;
		i = -i;
	}
	while (i > 9)
	{
		i = i / 10;
		count++;
	}
	return (count + 1);
}

void	ft_sign(int *sign, long int *i, char **res)
{
	if (*i < 0)
	{
		*i = *i * -1;
		*res[0] = '-';
		*sign = 1;
	}
}

char	*ft_itoa(int n)
{
	char		*res;
	long int	i;
	int			k;
	int			sign;

	res = (char *)malloc(count_nb(n) * sizeof(char) + 1);
	if (!res)
		return (NULL);
	i = n;
	k = 0;
	sign = 0;
	ft_sign(&sign, &i, &res);
	while (i > 9)
	{
		res[count_nb(n) - k - 1] = i % 10 + '0';
		i = i / 10;
		k++;
	}
	res[sign] = i + '0';
	res[count_nb(n)] = '\0';
	return (res);
}

char	*fps_calculator(char *fps_str)
{
	static struct timeval	last_time = {0, 0};
	static int				frame_count = 0;
	struct timeval			current_time;
	double					elapsed;
	int						fps;

	gettimeofday(&current_time, NULL);
	frame_count++;
	elapsed = (current_time.tv_sec - last_time.tv_sec)
		+ (current_time.tv_usec - last_time.tv_usec) / 1000000.0;
	if (elapsed >= 1.0)
	{
		fps = (int)(frame_count / elapsed);
		fps_str = ft_itoa(fps);
		last_time = current_time;
		frame_count = 0;
	}
	return (fps_str);
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
	cube->fps_str = fps_calculator(cube->fps_str);
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img.img, 0, 0);
	mlx_string_put(cube->mlx, cube->win, 10, 10, 0x00FF00, cube->fps_str);
	return (0);
}
