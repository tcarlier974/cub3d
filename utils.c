/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:00:00 by tcarlier          #+#    #+#             */
/*   Updated: 2025/05/29 11:10:03 by tcarlier         ###   ########.fr       */
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

int	count_col(const char *file_path)
{
    int     fd;
    char    *line;
    int     count;

    fd = open(file_path, O_RDONLY);
    if (fd < 0)
        return (0);
    line = get_next_line(fd);
    if (!line)
    {
        close(fd);
        return (0);
    }
    count = 0;
    while (line[count] && line[count] != '\n')
        count++;
    free(line);
    close(fd);
    return (count);
}
