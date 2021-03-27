/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 16:51:47 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"
#include "history.h"

int		make_line(char *buf_join, char **line)
{
	int			i;

	i = 0;
	while (buf_join[i] != '\n' && buf_join[i] != '\0')
		i++;
	if (buf_join[i] == '\n')
	{
		*line = ft_substr(buf_join, 0, i);
		free(buf_join);
		return (TERM_READ);
	}
	*line = ft_strdup(buf_join);
	free(buf_join);
	return (TERM_EOF);
}

char	*term_backspace(char *buf_join, int *buf_len, int *cursor_len)
{
	char	*tmp;

	if (*buf_len == 0 || *cursor_len <= 0)
		return (buf_join);
	ft_putstr_fd(TERM_BACKSPACE, STDOUT_FILENO);
	*buf_len = (*buf_len - 1);
	if (*cursor_len > *buf_len)
	{
		*cursor_len = *buf_len;
		tmp = buf_join;
		buf_join = ft_substr(tmp, 0, *buf_len);
		free(tmp);
	}
	return (buf_join);
}

int		term_newline(char **line, char *buf_join)
{
	history(buf_join, 0);
	*line = ft_strdup(buf_join);
	free(buf_join);
	return (TERM_READ);
}

char	*term_join(char *buf_join, char *buf, int *buf_len, int *cursor_len)
{
	char	*tmp;

	*buf_len = (*buf_len + 1);
	*cursor_len = (*cursor_len + 1);
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
	return (buf_join);
}

int			terminal(char **line)
{
	char	buf[2];
	char	*buf_join;
	int		rc;
	int		buf_len;
	int		cursor_len;

	buf[1] = 0;
	buf_join = ft_strdup("");
	buf_len = 0;
	cursor_len = 0;
	while ((rc = read(STDIN_FILENO, buf, 1)) >= 0)
	{
		if (buf[0] == ESCAPE)
			buf_join = term_arrow(buf_join, &buf_len, &cursor_len);
		else if (buf[0] == BACKSPACE)
			buf_join = term_backspace(buf_join, &buf_len, &cursor_len);
		else if (buf[0] == '\n')
			return (term_newline(line, buf_join));
		else
			buf_join = term_join(buf_join, buf, &buf_len, &cursor_len);
	}
	return (make_line(buf_join, line));
}
