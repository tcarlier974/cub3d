/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:00:00 by tcarlier          #+#    #+#             */
/*   Updated: 2025/11/30 18:55:56 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/include.h"

void	count_lines_loop(char **line, int *j, int fd)
{
	while (*j < 6 && *line)
	{
		while (*line && (*line)[0] == '\n')
		{
			free(*line);
			*line = get_next_line(fd);
		}
		if (!*line)
			return ;
		if ((*line)[0] != '\n' && (*line)[0] != '\0')
		{
			(*j)++;
		}
		free(*line);
		*line = get_next_line(fd);
	}
}

int	count_lines(const char *file_path)
{
	int		fd;
	char	*line;
	int		count;
	int		j;

	j = 0;
	count = 0;
	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		return (0);
	line = get_next_line(fd);
	count_lines_loop(&line, &j, fd);
	while (line && (line[0] == '\n' || line[0] == '\0'))
	{
		free(line);
		line = get_next_line(fd);
	}
	while (line)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (count);
}

int	count_max_col_setup(const char *file_path, int *fd, char **line)
{
	*fd = open(file_path, O_RDONLY);
	if (*fd < 0)
		return (0);
	*line = get_next_line(*fd);
	if (!(*line))
	{
		close(*fd);
		return (0);
	}
	return (1);
}

int	count_max_col(const char *file_path)
{
	t_colcmp	col;

	col.j = 0;
	if (!count_max_col_setup(file_path, &col.fd, &col.line))
		return (0);
	count_lines_loop(&col.line, &col.j, col.fd);
	while (col.line && (col.line[0] == '\n' || col.line[0] == '\0'))
	{
		free(col.line);
		col.line = get_next_line(col.fd);
	}
	col.max = 0;
	while (col.line)
	{
		col.i = 0;
		while (col.line[col.i] && col.line[col.i] != '\n')
			col.i++;
		if (col.i > col.max)
			col.max = col.i;
		free(col.line);
		col.line = get_next_line(col.fd);
	}
	free(col.line);
	close(col.fd);
	return (col.max);
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}
