/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igilbert <igilbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 06:10:00 by igilbert          #+#    #+#             */
/*   Updated: 2025/12/02 16:17:04 by igilbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	init_map_process_char(t_init_tool *t, char ***map, t_cube *cube)
{
	if (t->ch == 'N' || t->ch == 'S' || t->ch == 'E' || t->ch == 'W')
	{
		if (cube->player_exists >= 1)
			cube->player_exists = 2;
		else
		{
			(*map)[t->line_count][t->i] = '0';
			cube->dir_x = (t->ch == 'E') - (t->ch == 'W');
			cube->dir_y = (t->ch == 'S') - (t->ch == 'N');
			cube->plane_x = -cube->dir_y * 0.66;
			cube->plane_y = cube->dir_x * 0.66;
			cube->player_x = (double)t->i + 0.5;
			cube->player_y = (double)t->line_count + 0.5;
			cube->player_exists = 1;
		}
	}
	else if (t->ch == ' ' || t->ch == '\t')
		(*map)[t->line_count][t->i] = '0';
	else if (t->ch >= '0' && t->ch < '4')
		(*map)[t->line_count][t->i] = t->ch;
	else
	{
		cube->wrongmap = 1;
		(*map)[t->line_count][t->i] = '0';
	}
}

void	init_map_parse_line(t_init_tool *t, char ***map, t_cube *cube)
{
	while (t->l < cube->map_width)
	{
		(*map)[t->line_count][t->l] = '0';
		t->l++;
	}
	(*map)[t->line_count][cube->map_width] = '\0';
	while (t->i < (int)t->len && t->i < cube->map_width
		&& t->line[t->i] != '\n')
	{
		t->ch = t->line[t->i];
		init_map_process_char(t, map, cube);
		t->i++;
	}
}

void	skip_texture_lines(t_init_tool *t)
{
	while (t->j < 6 && t->line)
	{
		while (t->line && (t->line[0] == '\n' || t->line[0] == '\0'))
		{
			free(t->line);
			t->line = get_next_line(t->fd);
		}
		if (!t->line)
			break ;
		if (t->line[0] != '\n' && t->line[0] != '\0')
			t->j++;
		free(t->line);
		t->line = get_next_line(t->fd);
	}
}

int	alloc_map_line(t_init_tool *t, char ***map, t_cube *cube)
{
	(*map)[t->line_count] = malloc(sizeof(char) * (cube->map_width + 1));
	if (!(*map)[t->line_count])
	{
		free(t->line);
		while (t->k < t->line_count)
		{
			free((*map)[t->k]);
			t->k++;
		}
		free(*map);
		close(t->fd);
		return (0);
	}
	return (1);
}

void	process_map_lines(t_init_tool *t, char ***map, t_cube *cube)
{
	while (t->line && (t->line[0] == '\n' || t->line[0] == '\0'))
	{
		free(t->line);
		t->line = get_next_line(t->fd);
	}
	while (t->line)
	{
		t->i = 0;
		t->l = 0;
		t->len = ft_strlen(t->line);
		if (!alloc_map_line(t, map, cube))
			return ;
		init_map_parse_line(t, map, cube);
		t->line_count++;
		free(t->line);
		t->line = get_next_line(t->fd);
	}
	(*map)[t->line_count] = NULL;
}
