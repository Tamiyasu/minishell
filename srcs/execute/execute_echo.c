/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 17:35:44 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/08 17:55:22 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

static void	echo_with_options(t_lsttoken *token)
{
	token = token->next;
	while (token != NULL)
	{
		ft_putstr_fd(token->data, 1);
		token = token->next;
		if (token ==  NULL)
		{
			break ;
		}
		ft_putchar_fd(' ', 1);
	}
	return ;
}

void	execute_echo(t_lsttoken *token)
{
	token = token->next;
	if (token == NULL)
	{
		ft_putendl_fd("", 1);
		return ;
	}
	if (ft_strcmp(token->data, "-n") == 0)
	{
		echo_with_options(token);
		return ;
	}
	while (token != NULL)
	{
		ft_putstr_fd(token->data, 1);
		token = token->next;
		if (token ==  NULL)
		{
			break ;
		}
		ft_putchar_fd(' ', 1);
	}
	ft_putchar_fd('\n', 1);
	return ;
}
