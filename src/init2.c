/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:28:08 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/16 20:00:30 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	texture_recup(t_cube *cube, char ***textures, int i)
{
	if (!(*textures)[i])
	{
		fprintf(stderr, "Texture path not found for index %d\n", i);
		exit(EXIT_FAILURE);
	}
	printf("Loading texture %d: %s\n", i, (*textures)[i]);
	cube->texture[i].img = NULL;
	cube->texture[i].bits_per_pixel = 0;
	cube->texture[i].line_length = 0;
	cube->texture[i].endian = 0;
	cube->texture[i].width = 0;
	cube->texture[i].height = 0;
	cube->texture[i].img = mlx_xpm_file_to_image(cube->mlx, (*textures)[i],
			&cube->texture[i].width, &cube->texture[i].height);
	if (!cube->texture[i].img)
	{
		fprintf(stderr, "Failed to load texture: %s\n", (*textures)[i]);
		exit(EXIT_FAILURE);
	}
	free((*textures)[i]);
	cube->texture[i].addr = mlx_get_data_addr(cube->texture[i].img,
			&cube->texture[i].bits_per_pixel, &cube->texture[i].line_length,
			&cube->texture[i].endian);
	printf("Texture %d loaded: %dx%d\n", i, cube->texture[i].width,
		cube->texture[i].height);
}
