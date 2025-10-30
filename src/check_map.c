/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:34:55 by tcarlier          #+#    #+#             */
/*   Updated: 2025/10/30 19:35:43 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

int	check_spawn(t_cube *cube)
{
	if ((int)cube->player_x < 0 || (int)cube->player_x >= cube->map_width
		|| (int)cube->player_y < 0 || (int)cube->player_y >= cube->map_height)
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
	return (1);
}

void	free_checker(char ***visited, t_cube *cube)
{
	int	i;

	i = 0;
	while (i < cube->map_height)
	{
		free((*visited)[i]);
		i++;
	}
	free(*visited);
}

int	init_visit(char ***visited, t_cube *cube)
{
	int	i;

	i = 0;
	*visited = malloc(sizeof(char *) * cube->map_height);
	if (!(*visited))
		return (0);
	while (i < cube->map_height)
	{
		(*visited)[i] = malloc(sizeof(char) * (cube->map_width + 1));
		if (!(*visited)[i])
		{
			while (--i >= 0)
				free((*visited)[i]);
			free((*visited));
			return (0);
		}
		memset((*visited)[i], '0', cube->map_width);
		(*visited)[i][cube->map_width] = '\0';
		i++;
	}
	return (1);
}

int	validate_map_closed(t_cube *cube)
{
	char	**visited;
	int		result;

	if (!check_spawn(cube))
		return (0);
	if (!init_visit(&visited, cube))
		return (0);
	printf("c'est bieng fermé ??? (%d, %d)...\n",
		(int)cube->player_x, (int)cube->player_y);
	result = bcktrck(cube, (int)cube->player_x, (int)cube->player_y, visited);
	free_checker(&visited, cube);
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
	if ((x == 0 || x == cube->map_width - 1 || y == 0
			|| y == cube->map_height - 1)
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
