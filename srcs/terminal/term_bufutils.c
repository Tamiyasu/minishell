/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term_bufutils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:03:16 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/27 20:04:15 by tmurakam         ###   ########.fr       */
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
