/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 12:39:32 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "signal_handler.h"
#include "expansion.h"

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
