/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 21:36:39 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"

int		make_line(char *buf_join, char **line)
{
	int	i;

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

int		terminal(char **line)
{
	char	buf[2];
	char	*buf_join;
	int		rc;
	int		buf_len;
	int		cursor_len;

	buf[1] = 0;
	buf_join = NULL;
	init_buf(&buf_join, &buf_len, &cursor_len);
	while ((rc = read(STDIN_FILENO, buf, 1)) >= 0)
	{
		if (last_signal(0))
			init_buf(&buf_join, &buf_len, &cursor_len);
		if (buf[0] == ESCAPE)
			buf_join = term_history(buf_join, &buf_len, &cursor_len);
		else if (buf[0] == BACKSPACE)
			buf_join = term_backspace(buf_join, &buf_len, &cursor_len);
		else if (buf[0] == '\n')
			return (term_newline(line, buf_join));
		else if (buf[0] == EOT && buf_len == 0)
			break ;
		else if (buf[0] != EOT)
			buf_join = term_join(buf_join, buf, &buf_len, &cursor_len);
	}
	return (make_line(buf_join, line));
}
