/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 12:47:47 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 12:49:39 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int				is_redirect(int flag)
{
	return (
		flag == FT_REDIRECT_O_F ||
		flag == FT_REDIRECT_A_F ||
		flag == FT_REDIRECT_I_F);
}

int				check_input(int c_type, int last_type)
{
	if (last_type == FT_EMPTY_F ||
		last_type == FT_PIPE_F ||
		last_type == FT_SEMICOLON_F ||
		is_redirect(last_type))
	{
		if (c_type == FT_PIPE_F)
			return (0);
		if (c_type == FT_SEMICOLON_F)
			return (0);
	}
	if (is_redirect(last_type))
	{
		if (is_redirect(c_type))
		{
			return (0);
		}
	}
	return (1);
}

int				check_last_input(int c_type)
{
	if (c_type == FT_PIPE_F)
		return (0);
	if (is_redirect(c_type))
		return (0);
	return (1);
}
