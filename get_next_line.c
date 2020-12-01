/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pyasuko <pyasuko@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 19:42:46 by pyasuko           #+#    #+#             */
/*   Updated: 2020/12/01 11:03:14 by pyasuko          ###   ########.fr       */
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

static int	shift(int i, int fd, char **remain)//сдвигаем вперёд,
{											//чтобы от \n не терялось начало
	char *tmp;

	tmp = ft_strdup(remain[fd] + i + 1);//делаем копию строки, сдигаем на индекс \n и +1
	if (tmp == NULL)
		return (fail_free(fd, remain));
	free(remain[fd]);
	remain[fd] = tmp;
	return (1);
}

static int	division(int fd, char **line, char **remain)//мы считываем по размеру буф,
{														//если в строке есть \n, разделяем
	int i;

	i = 0;
	while (remain[fd][i] != '\n' && remain[fd][i] != '\0')
		i++;
	if (remain[fd][i] == '\n')
	{
		*line = ft_substr(remain[fd], 0, i);//подрезаем от начала до \n
		if (*line == NULL)
			return (fail_free(fd, remain));
	}
	else if (remain[fd][i] == '\0')
	{
		*line = ft_strdup(remain[fd]);//копируем полностью
		if (*line == NULL)
			return (fail_free(fd, remain));
		free(remain[fd]);
		remain[fd] = NULL;
		return (0);
	}
	return (shift(i, fd, remain));//сдвиг
}

int			get_next_line(int fd, char **line)
{
	char			buf[BUFFER_SIZE + 1];//по сабджекту
	char			*tmp;//для затирания промежуточного результата после склеек strjoin()
	int				byte_was_read;//для значения read()
	static char		*remain[OPEN_MAX];//для хранения дескрипторов и потоков с них

	if (fd < 0 || !line || BUFFER_SIZE <= 0 || read(fd, buf, 0) < 0)
		return (fail_free(fd, remain));
	if (!remain[fd])//инициализация массива, иначе - сега
		remain[fd] = ft_strdup("");
	while (!(ft_strchr(remain[fd], '\n')) &&//пока нет вхождения \n
		(byte_was_read = read(fd, buf, BUFFER_SIZE)) > 0)//если есть что читать
	{
		buf[byte_was_read] = '\0';//ставим конец строки
		tmp = ft_strjoin(remain[fd], buf);//склеиваем то что прочитали
		if (!tmp)
			return (fail_free(fd, remain));
		free(remain[fd]);
		remain[fd] = tmp;//вставляем целиком строку
	}
	return (division(fd, line, remain));//деление
}
