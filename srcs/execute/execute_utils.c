/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 12:41:41 by tmurakam          #+#    #+#             */
/*   Updated: 2021/04/04 17:43:28 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "signal_handler.h"
#include "expansion.h"

int		get_exit_status(int pid_status)
{
	if (WIFSIGNALED(pid_status))
		return (WTERMSIG(pid_status) + 128);
	else
		return (WEXITSTATUS(pid_status));
}

int		exec_is_builtin(char *token_data)
{
	if (ft_strcmp(token_data, "cd") == 0
		|| ft_strcmp(token_data, "echo") == 0
		|| ft_strcmp(token_data, "env") == 0
		|| ft_strcmp(token_data, "exit") == 0
		|| ft_strcmp(token_data, "export") == 0
		|| ft_strcmp(token_data, "pwd") == 0
		|| ft_strcmp(token_data, "unset") == 0)
	{
		return (1);
	}
	return (0);
}

void	command_builtin(t_token *token, t_env *env)
{
	if (ft_strcmp(token->data, "cd") == 0)
		g_exit_status = command_cd(token, env);
	else if (ft_strcmp(token->data, "echo") == 0)
		g_exit_status = command_echo(token);
	else if (ft_strcmp(token->data, "env") == 0)
		g_exit_status = command_env(env->data);
	else if (ft_strcmp(token->data, "export") == 0)
		g_exit_status = command_export(token, env);
	else if (ft_strcmp(token->data, "pwd") == 0)
		g_exit_status = command_pwd(env);
	else if (ft_strcmp(token->data, "unset") == 0)
		g_exit_status = command_unset(token, env);
	else if (ft_strcmp(token->data, "exit") == 0)
		command_exit(token);
	return ;
}

void	set_signals(void (*fc)(int))
{
	signal(SIGINT, fc);
	signal(SIGQUIT, fc);
}

void	fd_reset_and_free(t_info_fd *msh_fd)
{
	reset_fd(msh_fd);
	free_fd(&msh_fd);
}
