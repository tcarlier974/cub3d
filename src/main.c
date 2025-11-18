/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/18 11:20:18 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	mlx_shit(t_cube *cube)
{
	raycast(cube);
	mlx_put_image_to_window(cube->mlx, cube->win, cube->img.img, 0, 0);
	mlx_do_key_autorepeaton(cube->mlx);
	mlx_hook(cube->win, 17, 0, ft_mlx_loop_end, cube);
	mlx_hook(cube->win, 2, 1L << 0, key_hook_press, cube);
	mlx_hook(cube->win, 3, 1L << 1, key_release_hook, cube);
	mlx_loop_hook(cube->mlx, update_game_state, cube);
	mlx_loop(cube->mlx);
}

int	main_mes(int code, int j, char **av, t_cube *cube)
{
	if (code == 1)
	{
		if (count_lines(av[1]) <= 0 || count_max_col(av[1]) <= 0)
		{
			write(2, "Invalid map file\n", 18);
			return (0);
		}
	}
	else
	{
		if (!validate_map_closed(cube))
		{
			write(2, "Error: map not properly closed\n", 32);
			while (j < cube->map_height)
			{
				free(cube->map[j]);
				j++;
			}
			free(cube->map);
			return (0);
		}
	}
	return (1);
}

int	main(int ac, char **av)
{
	t_cube	cube;

	if (ac != 2)
	{
		write(2, "Usage: ./cub3D <map_file>\n", 27);
		return (1);
	}
	if (!main_mes(1, 0, av, &cube))
		return (1);
	init_cube(&cube, av[1]);
	recup_texture(&cube, av[1]);
	init_map(&cube.map, av[1], &cube);
	if (!main_mes(2077, 0, av, &cube))
		return (1);
	mlx_shit(&cube);
	return (0);
}
