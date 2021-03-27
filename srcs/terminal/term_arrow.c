/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_arrow.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 16:01:39 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 16:29:20 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"
#include "history.h"

char	*term_history(char *buf_join, char key, int *buf_len, int *cursor_len)
{
	char	*tmp;

	tmp = buf_join;
	if (key == 'A')
		buf_join = history(tmp, 1);
	else
		buf_join = history(tmp, -1);
	free(tmp);
	*buf_len = ft_strlen(buf_join);
	*cursor_len = *buf_len;
	ft_putstr_fd(TERM_CLEAR, STDOUT_FILENO);
	ft_putstr_fd("minishell>> ", STDOUT_FILENO);
	ft_putstr_fd(buf_join, STDOUT_FILENO);
	return (buf_join);
}

void	term_cursor(char key, int *cursor_len, int buf_len)
{
	if (key == 'C')
	{
		if (*cursor_len < buf_len)
		{
			*cursor_len = (*cursor_len + 1);
			ft_putstr_fd(TERM_RIGHT, STDOUT_FILENO);
		}
	}
	else
	{
		if (*cursor_len > 0)
		{
			*cursor_len = (*cursor_len - 1);
			ft_putstr_fd(TERM_LEFT, STDOUT_FILENO);
		}
	}
}

char	*term_arrow(char *buf_join, int *buf_len, int *cursor_len)
{
	char	buf[2];

	buf[1] = 0;
	read(STDIN_FILENO, buf, 1);
	read(STDIN_FILENO, buf, 1);
	if (buf[0] == 'A' || buf[0] == 'B')
		buf_join = term_history(buf_join, buf[0], buf_len, cursor_len);
	else if (buf[0] == 'C' || buf[0] == 'D')
		term_cursor(buf[0], cursor_len, *buf_len);
	return (buf_join);
}
