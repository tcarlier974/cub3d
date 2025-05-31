/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:00:00 by tcarlier          #+#    #+#             */
/*   Updated: 2025/05/31 10:39:57 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/include.h"

int	count_lines(const char *file_path)
{
    int     fd;
    char    *line;
    int     count;

    count = 0;
    fd = open(file_path, O_RDONLY);
    if (fd < 0)
        return (0);
    line = get_next_line(fd);
    while (line)
    {
        count++;
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (count);
}

int	count_max_col(const char *file_path)
{
    int     fd;
    char    *line;
    int     max;
    int     i;

    fd = open(file_path, O_RDONLY);
    if (fd < 0)
        return (0);
    line = get_next_line(fd);
    if (!line)
    {
        close(fd);
        return (0);
    }
    max = 0;
	while (line)
	{
		i = 0;
		while (line[i] && line[i] != '\n')
			i++;
		if (i > max)
			max = i;
		free(line);
		line = get_next_line(fd);
	}
    free(line);
    close(fd);
    return (max);
}
