/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 11:29:22 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "history.h"
#include <stdio.h>

# define ESCAPE '\33'
# define BACKSPACE '\177'

static int			make_line(char *buf_join, char **line)
{
	int			i;

	i = 0;
	while (buf_join[i] != '\n' && buf_join[i] != '\0')
		i++;
	if (buf_join[i] == '\n')
	{
		*line = ft_substr(buf_join, 0, i);
		free(buf_join);
		return (GNL_READ);
	}
	*line = ft_strdup(buf_join);
	free(buf_join);
	return (GNL_EOF);
}

int					get_next_line(char **line)
{
	char	buf[2];
	char	*buf_join;
	char	*tmp;
	int		rc;
	int		buf_len;
	int		cursor_idx;

	buf[1] = 0;
	buf_join = ft_strdup("");
	buf_len = 0;
	cursor_idx = 0;
	while ((rc = read(STDIN_FILENO, buf, 1)) >= 0)
	{
		if (buf[0] == ESCAPE)
		{
			read(STDIN_FILENO, buf, 1);// [ を読み取るよ
			read(STDIN_FILENO, buf, 1);// A を読み取るよ
			if (buf[0] == 'A')
			{
				tmp = buf_join;
				buf_join = history(tmp, 1);
				free(tmp);
				buf_len = ft_strlen(buf_join);
				cursor_idx = buf_len;
				ft_putstr_fd("\e[2K\e[G", STDOUT_FILENO);
				ft_putstr_fd("minishell>> ", STDOUT_FILENO);
				write(STDOUT_FILENO, buf_join, buf_len);
			}
			else if (buf[0] == 'B')
			{
				tmp = buf_join;
				buf_join = history(tmp, -1);
				free(tmp);
				buf_len = ft_strlen(buf_join);
				cursor_idx = buf_len;
				ft_putstr_fd("\e[2K\e[G", STDOUT_FILENO);
				ft_putstr_fd("minishell>> ", STDOUT_FILENO);
				write(STDOUT_FILENO, buf_join, buf_len);
			}
			else if (buf[0] == 'C')//right
			{
				if (cursor_idx >= 0 && cursor_idx < buf_len)
				{
					cursor_idx++;
					write(STDOUT_FILENO, "\e[1C", ft_strlen("\e[1C"));
				}
			}
			else if (buf[0] == 'D')//left
			{
				if (cursor_idx > 0 && cursor_idx <= buf_len)
				{
					cursor_idx--;
					write(STDOUT_FILENO, "\e[1D", ft_strlen("\e[1D"));
				}
			}
		}
		else if (buf[0] == BACKSPACE)
		{
			//printf("push backspace=[%d]\n", cursor_idx);
			if (buf_len == 0 || cursor_idx <= 0)
				continue ;
			// write(STDOUT_FILENO, "\10\e[1P", ft_strlen("\10\e[1P"));
			write(STDOUT_FILENO, "\b\e[K", ft_strlen("\b\e[K"));
			// write(2, "\b", ft_strlen("\b"));// "printf(hello worl\bd\n");->hello worl->hello word
			// write(2, "\e[K", ft_strlen("\e[K"));
			//write(STDOUT_FILENO, buf_join, buf_len);


			buf_len--;
			tmp = buf_join;
			buf_join = ft_substr(tmp, 0, buf_len);
			free(tmp);
		}
		else if (buf[0] == '\n')
		{
			history(buf_join, 0);
			//if (buf_join == NULL)
			//	*line = ft_strdup("\0");
			//else
			//{
			*line = ft_strdup(buf_join);
			free(buf_join);
			//}
			return (GNL_READ);
		}
		else
		{
			buf_len++;
			cursor_idx++;
			write(STDOUT_FILENO, buf, 1);
			if (buf_join == NULL)
				buf_join = ft_strdup(buf);
			else
			{
				tmp = ft_strjoin(buf_join, buf);
				free(buf_join);
				buf_join = ft_strdup(tmp);
				free(tmp);
			}
		}
	}
	return (make_line(buf_join, line));
}
