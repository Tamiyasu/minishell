/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 16:01:39 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 19:56:40 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"
#include "history.h"

char	*term_history(char *buf_join, int *buf_len, int *cursor_len)
{
	char	buf[2];
	char	*tmp;

	buf[1] = 0;
	read(STDIN_FILENO, buf, 1);
	read(STDIN_FILENO, buf, 1);
	tmp = buf_join;
	if (buf[0] == 'A')
		buf_join = history(tmp, 1);
	else if (buf[0] == 'B')
		buf_join = history(tmp, -1);
	else
		return (buf_join);
	free(tmp);
	*buf_len = ft_strlen(buf_join);
	*cursor_len = *buf_len;
	ft_putstr_fd(TERM_CLEAR, STDERR_FILENO);
	ft_putstr_fd("minishell>> ", STDERR_FILENO);
	ft_putstr_fd(buf_join, STDERR_FILENO);
	return (buf_join);
}

char	*term_backspace(char *buf_join, int *buf_len, int *cursor_len)
{
	char	*tmp;

	if (*buf_len == 0 || *cursor_len <= 0)
		return (buf_join);
	ft_putstr_fd(TERM_BACKSPACE, STDERR_FILENO);
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
	ft_putstr_fd("\n", STDERR_FILENO);
	return (TERM_READ);
}

char	*term_join(char *buf_join, char *buf, int *buf_len, int *cursor_len)
{
	char	*tmp;

	*buf_len = (*buf_len + 1);
	*cursor_len = (*cursor_len + 1);
	write(STDERR_FILENO, buf, 1);
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
