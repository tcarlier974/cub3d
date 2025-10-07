/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 19:31:49 by tcarlier          #+#    #+#             */
/*   Updated: 2025/10/07 17:52:07 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

int	validate_map_closed(t_cube *cube)
{
    char	**visited;
    int		i;
    int		result;

    if ((int)cube->player_x < 0 || (int)cube->player_x >= cube->map_width || 
        (int)cube->player_y < 0 || (int)cube->player_y >= cube->map_height)
    {
        printf("Error: Spawn point (%d, %d) en dehors de la map\n", 
               (int)cube->player_x, (int)cube->player_y);
        return (0);
    }
    if (cube->map[(int)cube->player_y][(int)cube->player_x] != '0')
    {
        printf("Error: Spawn point invalide ('%c')\n", 
               cube->map[(int)cube->player_y][(int)cube->player_x]);
        return (0);
    }
    visited = malloc(sizeof(char *) * cube->map_height);
    if (!visited)
        return (0);
    
    i = 0;
    while (i < cube->map_height)
    {
        visited[i] = malloc(sizeof(char) * (cube->map_width + 1));
        if (!visited[i])
        {
            while (--i >= 0)
                free(visited[i]);
            free(visited);
            return (0);
        }
        memset(visited[i], '0', cube->map_width);
        visited[i][cube->map_width] = '\0';
        i++;
    }
    printf("c'est bieng fermé ??? (%d, %d)...\n", (int)cube->player_x, (int)cube->player_y);
    result = bcktrck(cube, (int)cube->player_x, (int)cube->player_y, visited);
    i = 0;
    while (i < cube->map_height)
    {
        free(visited[i]);
        i++;
    }
    free(visited);
    if (result)
        printf("map bien fermée\n");
    else
        printf("map pas bienfermée :3\n");
    return (result);
}

int	bcktrck(t_cube *cube, int x, int y, char **visited)
{
    if (x < 0 || x >= cube->map_width || y < 0 || y >= cube->map_height)
        return (0);
    if (visited[y][x] == '1' || cube->map[y][x] == '1')
        return (1);
    if ((x == 0 || x == cube->map_width - 1 || y == 0 || y == cube->map_height - 1) 
        && cube->map[y][x] != '1')
        return (0);
    visited[y][x] = '1';
    if (cube->map[y][x] != '1')
    {
        if (!bcktrck(cube, x + 1, y, visited))
            return (0);
        if (!bcktrck(cube, x - 1, y, visited))
            return (0);
        if (!bcktrck(cube, x, y + 1, visited))
            return (0);
        if (!bcktrck(cube, x, y - 1, visited))
            return (0);
    }
    return (1);
}

int	main(int ac, char **av)
{
    t_cube cube;
    int i = 0;

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
