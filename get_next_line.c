/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vuslysty <vuslysty@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 13:42:11 by vuslysty          #+#    #+#             */
/*   Updated: 2018/11/13 13:39:19 by vuslysty         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include <fcntl.h>
#include "get_next_line.h"

t_list				*ft_listnew(int fd)
{
	t_list			*list;

	list = (t_list*)ft_memalloc(sizeof(t_list));
	if (list)
	{
		list->next = NULL;
		list->content = (char*)ft_memalloc(BUFF_SIZE + 1);
		if (list->content)
			list->content_size = fd;
		else
		{
			free(list);
			return (NULL);
		}
	}
	return (list);
}

int					is_endline(t_list *list, char **all_str)
{
	int				i;
	char			*temp;
	char			buf[BUFF_SIZE + 1];
	char			*str_buf;

	str_buf = (char*)list->content;
	i = -1;
	temp = *all_str;
	while (str_buf[++i] != '\0')
		if (str_buf[i] == '\n')
		{
			ft_strncpy(buf, str_buf, i);
			buf[i] = '\0';
			*all_str = ft_strjoin(temp, buf);
			free(temp);
			ft_strncpy(str_buf, str_buf + i + 1, BUFF_SIZE);
			return (1);
		}
	*all_str = ft_strjoin(temp, str_buf);
	free(temp);
	ft_bzero(str_buf, BUFF_SIZE);
	return (0);
}

t_list				*get_right_list(int fd, t_list *begin)
{
	t_list			*temp;

	temp = begin;
	while (temp->next != NULL)
	{
		if ((int)temp->content_size == fd)
			return (temp);
		temp = temp->next;
	}
	if ((int)temp->content_size == fd)
		return (temp);
	else
		ft_lstadd_end(&begin, ft_listnew(fd));
	return (temp->next);
}

/*
** The function *validator* return 3 values: -1, 0, 1
** -1 - when discriptor is not valid;
** 0  - when file is valid and you can read it;
** 1  - when file descriptor moved to end
*/

int					validator(t_list *list, char **all_str)
{
	int				rd;
	int				fd;
	char			buf[1];

	fd = (int)list->content_size;
	if (fd >= 0)
	{
		rd = read(fd, buf, 1);
		if (rd == 1)
			((char*)list->content)[ft_strlen(((char*)list->content))] = *buf;
		if (rd == 0)
			free(*all_str);
		if (rd == 1 || rd == 0)
			return ((rd == 0) ? 1 : 0);
	}
	free(*all_str);
	return (-1);
}

int					get_next_line(const int fd, char **line)
{
	static t_list	list = {"", -1, NULL};
	t_list			*temp;
	char			*all_str;
	int				rd;
	int				valid;

	all_str = ft_strdup("");
	temp = get_right_list(fd, &list);
	valid = validator(temp, &all_str);
	if (((char*)(temp->content))[0] == '\0')
		if (valid)
			return ((valid == 1) ? 0 : -1);
	if (is_endline(temp, &all_str))
		rd = 1;
	else
		while ((rd = read(fd, (char*)temp->content, BUFF_SIZE)))
		{
			((char*)temp->content)[rd] = '\0';
			if (is_endline(temp, &all_str))
				break ;
		}
	*line = all_str;
	return ((rd == 0 && **line == '\0') ? 0 : 1);
}
