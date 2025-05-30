/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:27:54 by tcarlier          #+#    #+#             */
/*   Updated: 2024/12/09 03:24:55 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* get_next_line.c */
#include "get_next_line.h"
#include <stdio.h>

static char	*gnl_ft_strchr(const char *s, int c)
{
    while (*s)
    {
        if (*s == (char)c)
            return ((char *)s);
        s++;
    }
    return (NULL);
}

static char	*ft_extract_line(char **str)
{
    char	*line;
    char	*tmp;
    int		i;

    i = 0;
    while ((*str)[i] && (*str)[i] != '\n')
        i++;
    if ((*str)[i] == '\n')
        i++;
    line = gnl_ft_substr(*str, 0, i);
    tmp = gnl_ft_substr(*str, i, gnl_ft_strlen(*str) - i);
    free(*str);
    if (!tmp)
    {
        free(line);
        *str = NULL;
        return (NULL);
    }
    *str = tmp;
    return (line);
}

static char	*ft_realloc(char *old, size_t olds, size_t news)
{
    char	*new;
    size_t	i;

    new = malloc(olds + news + 1);
    if (!new)
        return (NULL);
    i = 0;
    while (i < olds)
    {
        new[i] = old[i];
        i++;
    }
    new[i] = '\0';
    free(old);
    return (new);
}

static void	ft_find_new(t_gnl *f, ssize_t *bytes, int fd)
{
    int	len;

    len = gnl_ft_strlen(f->buf);
    while (!gnl_ft_strchr(f->buf, '\n') && *bytes > 0)
    {
        f->buf = ft_realloc(f->buf, len, BUFFER_SIZE);
        if (!f->buf)
        {
            *bytes = -1;
            return;
        }
        *bytes = read(fd, f->buf + len, BUFFER_SIZE);
        if (*bytes > 0)
        {
            f->buf[len + *bytes] = '\0';
            len += *bytes;
        }
    }
}

char	*get_next_line(int fd)
{
    static t_gnl	f[1024];
    char			*line;
    ssize_t			bytes;

    if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
        return (NULL);
    ft_init(&f[fd]);
    line = NULL;
    bytes = 1;
    ft_find_new(&f[fd], &bytes, fd);
    if (f[fd].buf && *f[fd].buf)
        line = ft_extract_line(&f[fd].buf);
    if (!line || (!f[fd].buf || !*f[fd].buf))
        cleanup_fd(&f[fd]);
    return (line);
}
