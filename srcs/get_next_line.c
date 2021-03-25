/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/24 21:56:02 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

# define ESCAPE '\33'
# define BACKSPACE '\177'

static int			make_line(char *buf_join, char **line)
{
	int			i;

	i = 0;
	while (buf_join[i] != '\n' && buf_join[i] != '\0')
	{
		//printf("buf_join[%d]=[%c]\n", i, buf_join[i]);
		i++;
	}
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
	char	buf;
	char	*buf_join;
	char	*tmp;
	int		rc;
	int		buf_idx;

	buf_join = NULL;
	buf_idx = 0;
	while((rc = read(STDIN_FILENO, &buf, 1)) >= 0)
	{
		if (buf == ESCAPE)
		{
			read(STDIN_FILENO, &buf, 1);// [ を読み取るよ
			read(STDIN_FILENO, &buf, 1);// A を読み取るよ
			if (buf == 'A')
			{
				if (buf_join)
					free(buf_join);
				buf_join = ft_strdup("up");
				buf_idx = 2;
				ft_putstr_fd("\e[2K\e[G", STDOUT_FILENO);
				ft_putstr_fd("minishell>> ", STDOUT_FILENO);
				write(STDOUT_FILENO, "up", 2);//履歴を出力&&セット
			}
			else if (buf == 'B')
			{
				if (buf_join)
					free(buf_join);
				buf_join = ft_strdup("down");
				buf_idx = 4;
				ft_putstr_fd("\e[2K\e[G", STDOUT_FILENO);
				ft_putstr_fd("minishell>> ", STDOUT_FILENO);
				write(STDOUT_FILENO, "down", 4);//履歴を出力&&セット
			}
			else if (buf == 'C')//left
			{
				write(STDOUT_FILENO, "\e[1C", ft_strlen("\e[1C"));
			}
			else if (buf == 'D')//right
			{
				write(STDOUT_FILENO, "\e[1D", ft_strlen("\e[1D"));
			}
		}
		else if (buf == BACKSPACE)
		{
			if (buf_idx == 0)
				continue ;
			write(STDOUT_FILENO, "\10\e[1P", ft_strlen("\10\e[1P"));
			buf_idx--;
			tmp = ft_strdup(buf_join);
			free(buf_join);
			buf_join = ft_substr(tmp, 0, buf_idx);
		}
		else if (buf == '\n')
		{
			if (buf_join == NULL)
				*line = ft_strdup("\0");
			else
			{
				*line = ft_strdup(buf_join);
				free(buf_join);
			}
			return (GNL_READ);
		}
		else
		{
			buf_idx++;
			write(STDOUT_FILENO, &buf, 1);
			if (buf_join == NULL)
				buf_join = ft_strdup(&buf);
			else
			{
				tmp = ft_strjoin(buf_join, &buf);
				free(buf_join);
				buf_join = ft_strdup(tmp);
				free(tmp);
			}
		}
	}
	return (make_line(buf_join, line));
}
