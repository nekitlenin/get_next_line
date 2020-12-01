/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pyasuko <pyasuko@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 19:42:46 by pyasuko           #+#    #+#             */
/*   Updated: 2020/12/01 10:20:11 by pyasuko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	fail_free(int fd, char **remain)
{
	if (remain[fd])
	{
		free(remain[fd]);
		remain[fd] = NULL;
	}
	return (-1);
}

static int	shift(int i, int fd, char **remain)
{
	char *tmp;

	tmp = ft_strdup(remain[fd] + i + 1);
	if (tmp == NULL)
		return (fail_free(fd, remain));
	free(remain[fd]);
	remain[fd] = tmp;
	return (1);
}

static int	division(int fd, char **line, char **remain)
{
	int i;

	i = 0;
	while (remain[fd][i] != '\n' && remain[fd][i] != '\0')
		i++;
	if (remain[fd][i] == '\n')
	{
		*line = ft_substr(remain[fd], 0, i);
		if (*line == NULL)
			return (fail_free(fd, remain));
	}
	else if (remain[fd][i] == '\0')
	{
		*line = ft_strdup(remain[fd]);
		if (*line == NULL)
			return (fail_free(fd, remain));
		free(remain[fd]);
		remain[fd] = NULL;
		return (0);
	}
	return (shift(i, fd, remain));
}

int			get_next_line(int fd, char **line)
{
	char			buf[BUFFER_SIZE + 1];
	char			*tmp;
	int				byte_was_read;
	static char		*remain[OPEN_MAX];

	if (fd < 0 || !line || BUFFER_SIZE <= 0 || read(fd, buf, 0) < 0)
		return (fail_free(fd, remain));
	if (!remain[fd])
		remain[fd] = ft_strdup("");
	while (!(ft_strchr(remain[fd], '\n')) &&
		(byte_was_read = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[byte_was_read] = '\0';
		tmp = ft_strjoin(remain[fd], buf);
		if (!tmp)
			return (fail_free(fd, remain));
		free(remain[fd]);
		remain[fd] = tmp;
	}
	return (division(fd, line, remain));
}
