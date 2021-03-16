/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 17:35:44 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/16 12:07:52 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

static void	echo_with_options(t_lsttoken *token)
{
	token = token->next;
	while (token != NULL)
	{
		ft_putstr_fd(token->data, STDOUT_FILENO);
		token = token->next;
		if (token ==  NULL)
		{
			break ;
		}
		ft_putchar_fd(' ', STDOUT_FILENO);
	}
	return ;
}

int			execute_echo(t_lsttoken *token)
{
	token = token->next;
	if (token == NULL)
	{
		ft_putendl_fd("", STDOUT_FILENO);
		return (0);
	}
	if (ft_strcmp(token->data, "-n") == 0)
	{
		echo_with_options(token);
		return (0);
	}
	while (token != NULL)
	{
		ft_putstr_fd(token->data, STDOUT_FILENO);
		token = token->next;
		if (token ==  NULL)
		{
			break ;
		}
		ft_putchar_fd(' ', STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
