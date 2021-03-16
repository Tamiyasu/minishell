/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 16:08:22 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/16 17:27:24 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	output_error(char *err_command, char *err_string)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(err_command, STDERR_FILENO);
	ft_putstr_fd(": " , STDERR_FILENO);
	ft_putendl_fd(err_string, STDERR_FILENO);
}

void	output_no_filename(void)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(".: filename argument requirred", STDERR_FILENO);
	ft_putendl_fd(".: usage: . filename [arguments]", STDERR_FILENO);
}

void	output_error_exit_args(char	*exit_args)
{
		ft_putendl_fd("exit", STDERR_FILENO);
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(exit_args, STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
}
