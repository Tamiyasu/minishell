/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 17:35:44 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 22:09:46 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

void	echo_with_options(t_token *token)
{
	token = token->next;
	while (token != NULL)
	{
		ft_putstr_fd(token->data, STDOUT_FILENO);
		token = token->next;
		if (token == NULL)
			break ;
		ft_putchar_fd(' ', STDOUT_FILENO);
	}
	return ;
}

int		command_echo(t_token *token)
{
	token = token->next;
	if (token == NULL)
	{
		ft_putendl_fd("", STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	if (ft_strcmp(token->data, "-n") == 0)
	{
		echo_with_options(token);
		return (EXIT_SUCCESS);
	}
	while (token != NULL)
	{
		ft_putstr_fd(token->data, STDOUT_FILENO);
		token = token->next;
		if (token == NULL)
			break ;
		ft_putchar_fd(' ', STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
	return (EXIT_SUCCESS);
}