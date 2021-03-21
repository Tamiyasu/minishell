/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 12:32:36 by tmurakam         ###   ########.fr       */
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

void	exec_command(t_token *token, t_env *env, int child_flag)
{
	pid_t	child_p;
	int		pid_status;

	pid_status = 0;
	if (ft_strcmp(token->data, "") == 0)
		return ;
	if (exec_is_builtin(token->data))
	{
		command_builtin(token, env);
		return ;
	}
	if (child_flag)
		command_execve(token, env);
	set_signals(sig_handler_c);
	child_p = fork();
	if (child_p == 0)
		command_execve(token, env);
	c_pid(child_p);
	waitpid(child_p, &pid_status, 0);
	c_pid(0);
	if (pid_status == 2)
		ft_putendl_fd("", STDOUT_FILENO);
	else if (pid_status == 3)
		ft_putendl_fd("Quit: 3", STDOUT_FILENO);
	g_exit_status = get_exit_status(pid_status);
}

void	execute(t_parser_node *node, t_env *env, t_info_fd *msh_fd)
{
	if (node == NULL)
		return ;
	else if (node->content->flag == FT_SEMICOLON_F)
	{
		execute(node->l_node, env, msh_fd);
		reset_fd(msh_fd);
		free_fd(&msh_fd);
		expansion(node->r_node, env);
		execute(node->r_node, env, msh_fd);
	}
	else if (node->content->flag == FT_PIPE_F)
		exec_pipe(node, env, msh_fd);
	else if (node->content->flag == FT_REDIRECT_I_F
			|| node->content->flag == FT_REDIRECT_O_F
			|| node->content->flag == FT_REDIRECT_A_F)
	{
		exec_redirect(node, msh_fd, env, execute);
	}
	else if (node->content->flag == FT_COMMAND_F)
	{
		exec_command(node->content, env, 0);
		reset_fd(msh_fd);
		free_fd(&msh_fd);
	}
}
