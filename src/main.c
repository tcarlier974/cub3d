/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/10/30 19:35:26 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

int	main(int ac, char **av)
{
	t_cube	cube;
	int		i = 0;

	if (ac != 2)
	{
		fprintf(stderr, "Usage: %s <map_file>\n", av[0]);
		return (1);
	}
	if (count_lines(av[1]) <= 0 || count_max_col(av[1]) <= 0)
	{
		fprintf(stderr, "Invalid map file: %s\n", av[1]);
		return (1);
	}
	init_cube(&cube, av[1]);
	recup_texture(&cube, av[1]);
	init_map(&cube.map, av[1], &cube);
	if (!validate_map_closed(&cube))
	{
		fprintf(stderr, "Error: non non non la map est pas bien fermée :3\n");
		for (int j = 0; j < cube.map_height; j++)
			free(cube.map[j]);
		free(cube.map);
		return (1);
	}
	printf("Carte chargée:\n");
	for (int y = 0; y < cube.map_height; y++) {
		if (cube.map[y])
			printf("[%d] %s\n", y, cube.map[y]);
	}
	printf("Player initialized at position: %.2f, %.2f\n", cube.player_x, cube.player_y);
	printf("Direction: (%.2f, %.2f)\n", cube.dir_x, cube.dir_y);
	raycast(&cube);
	mlx_put_image_to_window(cube.mlx, cube.win, cube.img.img, 0, 0);
	mlx_do_key_autorepeaton(cube.mlx);
	mlx_hook(cube.win, 17, 0, ft_mlx_loop_end, &cube);
	mlx_hook(cube.win, 2, 1L << 0, key_hook_press, &cube);
	mlx_hook(cube.win, 3, 1L << 1, key_release_hook, &cube);
	mlx_loop_hook(cube.mlx, update_game_state, &cube);
	mlx_loop(cube.mlx);
	return (0);
}
