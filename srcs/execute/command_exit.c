/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 16:46:48 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 02:22:32 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

int			exit_check_integer(char *args)
{
	int	idx;

	idx = 0;
	if (args[idx] == '+' || args[idx] == '-')
	{
		idx++;
		if (ft_isdigit(args[idx]) != 1)
			return (0);
		idx++;
	}
	while (args[idx] != '\0')
	{
		if (ft_isdigit(args[idx]) != 1)
			return (0);
		idx++;
	}
	return (1);
}

int			exit_check_argsnum(t_token *token)
{
	int	num;

	num = 0;
	while (token != NULL)
	{
		num++;
		token = token->next;
	}
	return (num);
}

long long	exit_atoi(t_token *token)
{
	long long	num;
	int			sign;
	int			idx;

	num = 0;
	sign = 1;
	idx = 0;
	if (token->data[idx] == '-' || token->data[idx] == '+')
	{
		if (token->data[idx] == '-')
			sign = -1;
		idx++;
	}
	while ('0' <= token->data[idx] && token->data[idx] <= '9')
	{
		num = num * 10 + (token->data[idx] - '0');
		idx++;
		if (idx >= 20)
			break ;
	}
	num *= sign;
	if ((sign == 1 && num < 0) || (sign == -1 && num > 0))
		token->flag = -1;
	return (num);
}

void		exit_check_status(t_token *token)
{
	long long	status;

	status = exit_atoi(token);
	if (token->flag == -1)
	{
		output_error_exit_args(token->data);
		exit(EXIT_OUT_OF_RANGE_STATUS);
	}
	g_exit_status = status % 256;
	if (g_exit_status < 0)
		g_exit_status += 256;
}

void		command_exit(t_token *token)
{
	token = token->next;
	if (token == NULL)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	if (exit_check_integer(token->data) != 1)
	{
		output_error_exit_args(token->data);
		exit(EXIT_OUT_OF_RANGE_STATUS);
	}
	if (exit_check_argsnum(token) != 1)
	{
		output_error("exit", "too many arguments");
		g_exit_status = 1;
		return ;
	}
	exit_check_status(token);
	ft_putendl_fd("exit", STDOUT_FILENO);
	exit(g_exit_status);
}
