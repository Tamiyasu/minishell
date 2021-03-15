/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/16 01:24:35 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/stat.h>
#include "execute.h"
#include "libft.h"
#include "signal_handler.h"

t_info_fd	*fd_list_last(t_info_fd *fd)
{
	t_info_fd	*return_fd;

	return_fd = fd;
	while (return_fd && return_fd->next)
	{
		return_fd = return_fd->next;
	}
	return (return_fd);
}

int get_exit_status(int pid_status)
{
	if (WIFSIGNALED(pid_status))
		return (WTERMSIG(pid_status) + 128);
	else
		return (WEXITSTATUS(pid_status));
}

void	fd_list_addback(t_info_fd **fd, t_info_fd *new)
{
	if (*fd)
		fd_list_last(*fd)->next = new;
	else
			*fd = new;
}

t_info_fd	*fd_list_new(int fd_num, int fd_save)
{
	t_info_fd	*fd;

	fd = malloc(sizeof(t_info_fd));
	if (fd)
	{
		fd->fd_num = fd_num;
		fd->fd_save = fd_save;
		fd->next = NULL;
	}
	return (fd);
}

void	free_fd(t_info_fd **fd)
{

	t_info_fd *temp;
	t_info_fd *temp_next;

	temp = *fd;
	while (temp != NULL)
	{
		temp_next = temp->next;
		free(temp);
		temp = temp_next;
	}
	*fd = NULL;
}

void	reset_fd(t_info_fd *fd)
{
	if (!fd)
		return ;
	while (fd)
	{
		dup2(fd->fd_save, fd->fd_num);
		close(fd->fd_save);
		fd = fd->next;
	}
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

int	 redirect_check_fdnum(char *data)
{
	int	fd_num;

	fd_num = ft_atoi(data);
	if (fd_num == 0)
	{
		fd_num = 1;
	}
	return (fd_num);
}

void	exec_command(t_lsttoken *token, t_env *env, int *exit_status, int flag)
{
	pid_t	child_p;
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
	//pipeなし,そのままexecve実行すると./minishell自体が終わってしまうのでforkする必要あり。
	signal(SIGINT, sig_handler_c);
	signal(SIGQUIT, sig_handler_c);
	child_p = fork();
	if (child_p == 0)
	{
		command_execve(token, env);
	}
	c_pid(child_p);
	waitpid(child_p, &pid_status, 0);
	c_pid(0);

	if (pid_status == 2)
		ft_putendl_fd("", STDOUT_FILENO);
	else if (pid_status == 3)
		ft_putendl_fd("Quit: 3", STDOUT_FILENO);
	*exit_status = get_exit_status(pid_status);
}

int	redirect_file_open(char *file, int flag)
{
	int	open_fd;

	if (flag == FT_REDIRECT_I_F)
		open_fd = open(file, O_RDONLY);
	else if (flag == FT_REDIRECT_O_F)
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

t_info_fd	*redirect_save_fd(t_info_fd *fd, int fd_num)//dupで保存はSTDINやSTDOUTなどのfd_num, fd_numがすでに存在していたらなし
{
	t_info_fd *new_fd;

	if (fd == NULL)
	{
		fd = fd_list_new(fd_num, dup(fd_num));
		return (fd);
	}
	new_fd = fd_list_new(fd_num, dup(fd_num));
	fd_list_addback(&fd, new_fd);
	return (fd);
}

int	redirect_check_reserve(t_info_fd *fd, int fd_num)
{
	while (fd)
	{
		if (fd->fd_num == fd_num)
		{
			return (0);
		}
		fd = fd->next;
	}
	return (1);
}

void	exec_redirect(t_parser_node *node, t_info_fd *fd,
						t_env *env, int *exit_status,
						void (*func)(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *fd))
{
	int	fd_num;
	int open_fd;

	if (node->content->flag == FT_REDIRECT_I_F)
		fd_num = STDIN_FILENO;
	else
		fd_num = redirect_check_fdnum(node->content->data);
	open_fd = redirect_file_open(node->r_node->content->data, node->content->flag);
	if (!open_fd)
		return ;
	if (redirect_check_reserve(fd, fd_num))
	{
		fd = redirect_save_fd(fd, fd_num);
		dup2(open_fd, fd_num);
	}
	close(open_fd);
	func(node->l_node, env, exit_status, fd);
}

void	exec_pipe(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *fd)
{
	int		pipe_fd[2];
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
	else if (node->content->flag == FT_REDIRECT_I_F
			|| node->content->flag == FT_REDIRECT_O_F
			|| node->content->flag == FT_REDIRECT_A_F)
	{
		exec_redirect(node, fd, env, exit_status, exec_pipe);
	}
	else if (node->content->flag == FT_PIPE_F)
	{
		pipe(pipe_fd);
		signal(SIGINT, sig_handler_c);
		signal(SIGQUIT, sig_handler_c);
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
		c_pid(child_p1);
		waitpid(child_p1, &status, 0);
		c_pid(child_p2);
		waitpid(child_p2, &status, 0);
		c_pid(0);
		if (status == 2)
			ft_putendl_fd("", STDOUT_FILENO);
		else if (status == 3)
			ft_putendl_fd("Quit: 3", STDOUT_FILENO);
		*exit_status = get_exit_status(status);
	}
}

void	execute(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *fd)
{
	if (node == NULL)
		return ;
	else if (node->content->flag == FT_SEMICOLON_F)
	{
		printf("------------------------ : \n");
		execute(node->l_node, env, exit_status, fd);
		reset_fd(fd);
		free_fd(&fd);
		execute(node->r_node, env, exit_status, fd);
		printf("exit_status : %d\n", *exit_status);
	}
	else if (node->content->flag == FT_PIPE_F)
		exec_pipe(node, env, exit_status, fd);
	else if (node->content->flag == FT_REDIRECT_I_F
			|| node->content->flag == FT_REDIRECT_O_F
			|| node->content->flag == FT_REDIRECT_A_F)
	{
		exec_redirect(node, fd, env, exit_status, execute);
	}
	else if (node->content->flag == FT_COMMAND_F)
	{
		exec_command(node->content, env, exit_status, 0);
		reset_fd(fd);
	}
}
