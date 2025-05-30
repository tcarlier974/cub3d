/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcarlier <tcarlier@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:27:52 by tcarlier          #+#    #+#             */
/*   Updated: 2024/12/09 03:14:17 by tcarlier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif

typedef struct s_gnl
{
	char	*buf;
	int		eof;
}	t_gnl;

char	*get_next_line(int fd);
char	*gnl_ft_strdup(const char *s);
void	ft_init(t_gnl *f);
size_t	gnl_ft_strlen(const char *s);
char	*gnl_ft_substr(char *s, unsigned int start, size_t len);
void	cleanup_fd(t_gnl *f);

#endif