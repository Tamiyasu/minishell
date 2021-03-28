/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_bufutils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:03:16 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/28 21:23:44 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "terminal.h"

void	init_buf(char **buf_join, int *buf_len, int *cursor_len)
{
	free(*buf_join);
	*buf_join = ft_strdup("");
	*buf_len = 0;
	*cursor_len = 0;
}

void	buf_signal(char **buf_join, int *buf_len, int *cursor_len)
{
	history(*buf_join, HISTORY_POS_RESET);
	init_buf(buf_join, buf_len, cursor_len);
}