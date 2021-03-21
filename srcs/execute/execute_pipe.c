/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 12:33:21 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/21 15:39:42 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "signal_handler.h"
#include "expansion.h"

void	exec_pipe_p1(int *pipe_fd, t_parser_node *node,
			t_env *env, t_info_fd **msh_fd)
{
	close(pipe_fd[READ]);
	dup2(pipe_fd[WRITE], STDOUT_FILENO);
	close(pipe_fd[WRITE]);
	exec_pipe(node->l_node, env, *msh_fd);
	exit(g_exit_status);
}

void	exec_pipe_p2(int *pipe_fd, t_parser_node *node,
			t_env *env, t_info_fd **msh_fd)
{
	close(pipe_fd[WRITE]);
	dup2(pipe_fd[READ], STDIN_FILENO);
	close(pipe_fd[READ]);
	exec_pipe(node->r_node, env, *msh_fd);
	exit(g_exit_status);
}

int		wait_and_getstatus(pid_t child_p1, pid_t child_p2)
{
	int status;
	int killed_flag = 0;

	c_pid(child_p1);
	waitpid(child_p1, &status, 0);
	if (status == 2)
		killed_flag = 1;
	c_pid(child_p2);
	waitpid(child_p2, &status, 0);
	c_pid(0);
	if (status == 2 || killed_flag == 1)
		ft_putendl_fd("", STDOUT_FILENO);
	else if (status == 3)
		ft_putendl_fd("Quit: 3", STDOUT_FILENO);
	return (status);
}

void	exec_pipe_command(t_parser_node *node, t_env *env, t_info_fd **msh_fd)
{
	exec_command(node->content, env, 1);
	free_fd(msh_fd);
}

void	exec_pipe(t_parser_node *node, t_env *env, t_info_fd *msh_fd)
{
	int		pipe_fd[2];
	pid_t	child_p1;
	pid_t	child_p2;

	if (node == NULL)
		return ;
	if (node->content->flag == FT_COMMAND_F)
		return (exec_pipe_command(node, env, &msh_fd));
	else if (is_redirect(node->content->flag))
		exec_redirect(node, msh_fd, env, exec_pipe);
	else if (node->content->flag == FT_PIPE_F)
	{
		pipe(pipe_fd);
		set_signals(sig_handler_c);
		child_p1 = fork();
		if (child_p1 == 0)
			exec_pipe_p1(pipe_fd, node, env, &msh_fd);
		child_p2 = fork();
		if (child_p2 == 0)
			exec_pipe_p2(pipe_fd, node, env, &msh_fd);
		close(pipe_fd[READ]);
		close(pipe_fd[WRITE]);
		g_exit_status = get_exit_status(wait_and_getstatus(child_p1, child_p2));
	}
}
