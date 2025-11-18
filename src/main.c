/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/18 12:19:52 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	ft_putstr_fd(int fd, char *str)
{
	int	i;

	if (!str || !fd)
		return ;
	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

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
			ft_putstr_fd(2, "Invalid map file\n");
			return (0);
		}
	}
	else
	{
		if (!validate_map_closed(cube))
		{
			ft_putstr_fd(2, "Error: map not properly closed\n");
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
		ft_putstr_fd(2, "Usage: ./cub3D <map_file>\n");
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
