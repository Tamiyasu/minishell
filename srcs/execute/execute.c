/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/14 01:17:59 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "execute.h"
#include "libft.h"
#include "signal_handler.h"

void	init_fd(t_info_fd *fd)
{
	// fd->redirect_i = -1;
	// fd->redirect_o = -1;
	// fd->redirect_err = -1;
	// fd->save_stdin = -1;
	// fd->save_stdout = -1;
	fd->save_stdin = dup(STDIN_FILENO);
	fd->save_stdout = dup(STDOUT_FILENO);
	fd->save_stderr = dup(STDERR_FILENO);
	fd->fd_num = -1;
}

void	reset_fd(t_info_fd *fd)
{
	dup2(fd->save_stdout, STDIN_FILENO);
	close(fd->save_stdin);
	dup2(fd->save_stdout, STDOUT_FILENO);
	close(fd->save_stdout);
	dup2(fd->save_stderr, STDERR_FILENO);
	close(fd->save_stderr);
}


int	exec_check_builtin(char *token_data)
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

int	exec_check_command_path(char *command, int *exit_status)
{
	struct stat		stat_buf;

	if (ft_strchr(command, '/') == 0)
	{
		*exit_status = 127;
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		return (-1);
	}
	if (command[0] == '.')
	{
		return (0);
	}
	lstat(command, &stat_buf);
	if ((stat_buf.st_mode & S_IFMT) == 0040000)
	{
		*exit_status = 126;
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd(strerror(21), STDERR_FILENO);
		return (-1);
	}
	return (0);
}

/*
** コマンドを実行する　returnで終了ステータスを返す.
*/
void	command_builtin(t_lsttoken *token, t_env *env, int *exit_status)
{
	if (ft_strcmp(token->data, "cd") == 0)
	{
		*exit_status = execute_cd(token, env);
	}
	else if (ft_strcmp(token->data, "echo") == 0)
	{
		*exit_status = execute_echo(token);
	}
	else if (ft_strcmp(token->data, "env") == 0)
	{
		*exit_status = execute_env(env->data);
	}
	else if (ft_strcmp(token->data, "export") == 0)
	{
		*exit_status = execute_export(token, env);
	}
	else if (ft_strcmp(token->data, "pwd") == 0)
	{
		*exit_status = execute_pwd();
	}
	else if (ft_strcmp(token->data, "unset") == 0)
	{
		*exit_status = execute_unset(token, env);
	}
	else if (ft_strcmp(token->data, "exit") == 0)
	{
		execute_exit(token, exit_status);
	}
	return ;
}

void	 redirect_check_fdnum(t_info_fd *fd, char *data)
{
		fd->fd_num = ft_atoi(data);
		if (fd->fd_num == 0)
		{
			fd->fd_num = 1;
		}
}

void	exec_command(t_lsttoken *token, t_env *env, int *exit_status, int flag)
{
	//pid_t	child_p;
	int		pid_status;

	pid_status = 0;

	// printf("--------------------------------simple_command()\nfd->save_stdin[%d] redirect_i[%d] save_stdout[%d] redirect_o[%d] save_stderr[%d] redirect_err[%d]\n\n", fd->save_stdin, fd->redirect_i, fd->save_stdout, fd->redirect_o, fd->save_stderr, fd->redirect_err);
	if (ft_strcmp(token->data, "") == 0)
	{
		return ;
	}
	if (exec_check_builtin(token->data))
	{
		command_builtin(token, env, exit_status);
		return ;
	}
	//execve実行前に、commandがディレクトリ名で指定されていないかチェック
	if (exec_check_command_path(token->data, exit_status) == -1)
	{
		return ;
	}
	if (flag)//pipeがあり、子プロセスないでコマンドが実行される時(execveを実行するのにforkする必要なし)
	{
		command_execve(token, env);
	}
	// child_p = fork();
	// if (child_p == 0)
	// 	command_execve(token, env);
	// waitpid(child_p, &pid_status, 0);


	//pipeなし,そのままexecve実行すると./minishell自体が終わってしまうのでforkする必要あり。
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
	c_pid(fork());
	if (c_pid(-1) == 0)
	{
		signal(SIGINT, sig_handler_c);
		signal(SIGQUIT, sig_handler_c);
		command_execve(token, env);
	}
	waitpid(c_pid(-1), &pid_status, 0);
	if (pid_status == 2)
		ft_putendl_fd("", STDOUT_FILENO);
	else if (pid_status == 3)
		ft_putendl_fd("Quit: 3", STDOUT_FILENO);
	signal(SIGINT, sig_handler_p);
	signal(SIGQUIT, sig_handler_p);
	c_pid(0);
	*exit_status = WEXITSTATUS(pid_status);
	// printf("WIFEXITED=[%d] WEXITSTATIS=[%d]\n\n", WIFEXITED(pid_status), WEXITSTATUS(pid_status));
}

int	redirect_in_check_open(char *file)
{
	int	open_fd;

	open_fd = open(file, O_RDONLY);
	if (!open_fd)
	{
		ft_putendl_fd(strerror(errno), STDOUT_FILENO);
		return (open_fd);
	}
	return (open_fd);
}

int	redirect_out_check_open(char *file, int flag)
{
	int	open_fd;

	if (flag == FT_REDIRECT_O_F)
		open_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else
		open_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (!open_fd)
	{
		ft_putendl_fd(strerror(errno), STDOUT_FILENO);
		return (open_fd);
	}
	return (open_fd);
}

void	exec_pipe(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *fd)
{
	int		pipe_fd[2];
	int		open_fd;
	int		status;
	pid_t	child_p1;
	pid_t	child_p2;

	if (node == NULL)
		return ;

	if (node->content->flag  == FT_COMMAND_F)
	{
		exec_command(node->content, env, exit_status, 1);
		return ;
	}
	else if (node->content->flag == FT_REDIRECT_I_F)
	{
		if ((open_fd = redirect_in_check_open(node->r_node->content->data))== -1)
			return ;
		dup2(open_fd, STDIN_FILENO);
		exec_pipe(node->l_node, env, exit_status, fd);
	}
	else if (node->content->flag == FT_REDIRECT_O_F || node->content->flag == FT_REDIRECT_A_F)
	{
		redirect_check_fdnum(fd, node->content->data);
		if ((open_fd = redirect_out_check_open(node->r_node->content->data, node->content->flag)) == -1)
			return ;
		if (fd->fd_num == STDERR_FILENO)
		{
			dup2(open_fd, STDERR_FILENO);
			close(open_fd);
		}
		else
		{
			dup2(open_fd, fd->fd_num);
			close(open_fd);
		}
		exec_pipe(node->l_node, env, exit_status, fd);
	}
	else if (node->content->flag == FT_PIPE_F)
	{
		pipe(pipe_fd);
		child_p1 = fork();
		if (child_p1 == 0)
		{
			// printf("next node->data[%s]\n",  node->l_node->content->data);
			close(pipe_fd[READ]);
			dup2(pipe_fd[WRITE], STDOUT_FILENO);
			close(pipe_fd[WRITE]);
			exec_pipe(node->l_node, env, exit_status, fd);
			exit(*exit_status);
		}
		child_p2 = fork();
		if (child_p2 == 0)
		{
			close(pipe_fd[WRITE]);
			dup2(pipe_fd[READ], STDIN_FILENO);
			close(pipe_fd[READ]);
			exec_pipe(node->r_node, env, exit_status, fd);
			exit(*exit_status);
		}
		close(pipe_fd[READ]);
		close(pipe_fd[WRITE]);
		waitpid(child_p1, &status, 0);
		waitpid(child_p2, &status, 0);
		*exit_status = WEXITSTATUS(status);
	}
}

void	execute(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *fd)
{
	int	open_fd;

	if (node == NULL)
	{
		return ;
	}
	if (node->content->flag == FT_SEMICOLON_F)
	{
		execute(node->l_node, env, exit_status, fd);
		reset_fd(fd);
		execute(node->r_node, env, exit_status, fd);
	}
	else if (node->content->flag == FT_PIPE_F)
		exec_pipe(node, env, exit_status, fd);
	else if (node->content->flag == FT_REDIRECT_I_F)
	{
		if ((open_fd = redirect_in_check_open(node->r_node->content->data)) == -1)
			return ;
		dup2(open_fd, STDIN_FILENO);
		close(open_fd);
		execute(node->l_node, env, exit_status, fd);
	}
	else if (node->content->flag == FT_REDIRECT_O_F || node->content->flag == FT_REDIRECT_A_F)
	{
		redirect_check_fdnum(fd, node->content->data);// >に指定された数字をfd->fd_numに保存
		if ((open_fd = redirect_out_check_open(node->r_node->content->data, node->content->flag)) == -1)
			return ;
		if (fd->fd_num == STDERR_FILENO)
		{
			dup2(open_fd, STDERR_FILENO);
			close(open_fd);
		}
		else
		{
			dup2(open_fd, fd->fd_num);
			close(open_fd);
		}
		execute(node->l_node, env, exit_status, fd);
	}
	if (node->content->flag == FT_COMMAND_F)
	{
		exec_command(node->content, env, exit_status, 0);
		reset_fd(fd);
	}
}
