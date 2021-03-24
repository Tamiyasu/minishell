/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/24 19:53:31 by ysaito           ###   ########.fr       */
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
		printf("buf_join[%d]=[%c]\n", i, buf_join[i]);
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
			read(STDIN_FILENO, &buf, 1);//[Aを読み取るよ
			read(STDIN_FILENO, &buf, 1);
			if (buf == 'A')
			{
				if (buf_join == NULL)
					buf_join = ft_strdup("up");
				else
				{
					tmp = ft_strjoin(buf_join, "up");
					free(buf_join);
					buf_join = ft_strdup(tmp);
					free(tmp);
				}
				buf_idx += 2;
				write(STDOUT_FILENO, buf_join, buf_idx);//履歴を出力&&セット
			}
			else if (buf == 'B')
			{
				if (buf_join == NULL)
					buf_join = ft_strdup("down");
				else
				{
					tmp = ft_strjoin(buf_join, "down");
					free(buf_join);
					buf_join = ft_strdup(tmp);
					free(tmp);
				}
				buf_idx += 4;
				write(STDOUT_FILENO, buf_join, buf_idx);//履歴を出力&&セット
			}
			else if (buf == 'C')
			{
				write(STDOUT_FILENO, "\33[C", 3);
			}
			else if (buf == 'D')
			{
				write(STDOUT_FILENO, "\10", 2);
			}
		}
		else if (buf == BACKSPACE)
		{
			if (buf_idx == 0)
				continue ;
			write(STDOUT_FILENO, "\10\33[1P", 5);
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
