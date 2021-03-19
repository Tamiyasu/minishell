/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 15:22:27 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "signal_handler.h"
#include "expansion.h"

t_info_fd	*fd_list_last(t_info_fd *msh_fd)
{
	t_info_fd	*return_fd;

	return_fd = msh_fd;
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

void	fd_list_addback(t_info_fd **msh_fd, t_info_fd *new)
{
	if (*msh_fd)
		fd_list_last(*msh_fd)->next = new;
	else
			*msh_fd = new;
}

t_info_fd	*fd_list_new(int fd_num, int fd_save, int flag)
{
	t_info_fd	*msh_fd;

	msh_fd = malloc(sizeof(t_info_fd));
	if (msh_fd)
	{
		msh_fd->fd_num = fd_num;
		msh_fd->fd_save = fd_save;
		msh_fd->flag = flag;
		msh_fd->next = NULL;
	}
	return (msh_fd);
}

void	free_fd(t_info_fd **msh_fd)
{

	t_info_fd *temp;
	t_info_fd *temp_next;

	temp = *msh_fd;
	while (temp != NULL)
	{
		temp_next = temp->next;
		free(temp);
		temp = temp_next;
	}
	*msh_fd = NULL;
}

void	reset_fd(t_info_fd *msh_fd)
{
	if (!msh_fd)
		return ;
	while (msh_fd)
	{
		dup2(msh_fd->fd_save, msh_fd->fd_num);
		close(msh_fd->fd_save);
		msh_fd = msh_fd->next;
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

void	command_builtin(t_lsttoken *token, t_env *env)
{
	if (ft_strcmp(token->data, "cd") == 0)
		g_exit_status = execute_cd(token, env);
	else if (ft_strcmp(token->data, "echo") == 0)
		g_exit_status = execute_echo(token);
	else if (ft_strcmp(token->data, "env") == 0)
		g_exit_status = execute_env(env->data);
	else if (ft_strcmp(token->data, "export") == 0)
		g_exit_status = execute_export(token, env);
	else if (ft_strcmp(token->data, "pwd") == 0)
		g_exit_status = execute_pwd();
	else if (ft_strcmp(token->data, "unset") == 0)
		g_exit_status = execute_unset(token, env);
	else if (ft_strcmp(token->data, "exit") == 0)
		execute_exit(token);
	return ;
}

char	*redirect_format_fdnum(char *data)
{
	char *format_data;
	int	i;

	i = 0;
	while (data[i])
	{
		if (!ft_isdigit(data[i]))
			break ;
		i++;
	}
	format_data = ft_substr(data, 0, i);
	printf("format_data=[%s]\n", format_data);
	free(data);
	return (format_data);
}

int	 redirect_check_fdnum(char *data, int redirect_flag)
{
	int	fd_num;

	fd_num = ft_atoi(data);
	printf("fd_num=[%d]\n", fd_num);
	if (fd_num == 0 && redirect_flag == FT_REDIRECT_O_F)
	{
		fd_num = 1;
	}
	return (fd_num);
}

void	exec_command(t_lsttoken *token, t_env *env, int child_flag)
{
	pid_t	child_p;
	int		pid_status;

	pid_status = 0;
	if (ft_strcmp(token->data, "") == 0)
		return ;
	if (exec_check_builtin(token->data))
	{
		command_builtin(token, env);
		return ;
	}
	if (child_flag)
		command_execve(token, env);
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
	g_exit_status = get_exit_status(pid_status);
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
	if (open_fd == -1)
		output_error(file, strerror(errno));
	return (open_fd);
}

t_info_fd	*redirect_save_fd(t_info_fd *msh_fd, int fd_num, int flag)
{
	t_info_fd *new_fd;

	if (msh_fd == NULL)
	{
		msh_fd = fd_list_new(fd_num, dup(fd_num), flag);
		return (msh_fd);
	}
	new_fd = fd_list_new(fd_num, dup(fd_num), flag);
	fd_list_addback(&msh_fd, new_fd);
	return (msh_fd);
}

int	redirect_check_reserve(t_info_fd *msh_fd, int fd_num, int redirect_flag)
{
	while (msh_fd)
	{
		if (redirect_flag == msh_fd->flag && fd_num == msh_fd->fd_num)
			return (0);
		msh_fd = msh_fd->next;
	}
	return (1);
}

void	exec_redirect(t_parser_node *node, t_info_fd *msh_fd, t_env *env,
						void (*func)(t_parser_node *node, t_env *env, t_info_fd *msh_fd))
{
	int	fd_num;
	int open_fd;

	fd_num = redirect_check_fdnum(node->content->data, node->content->flag);
	open_fd = redirect_file_open(node->r_node->content->data, node->content->flag);
	if (open_fd == -1 || fd_num > FD_MAX || fd_num < 0)
	{
		if (open_fd == -1)
			output_error("open", strerror(errno));
		else if (fd_num > FD_MAX)
		{
			node->content->data = redirect_format_fdnum(node->content->data);
			output_error(node->content->data, "Bad file descriptor");
		}
		else
			output_error("file descriptor out of range", "Bad file descriptor");
		g_exit_status = 1;
		return ;
	}
	if (redirect_check_reserve(msh_fd, fd_num, node->content->flag))
	{
		msh_fd = redirect_save_fd(msh_fd, fd_num, node->content->flag);
		dup2(open_fd, fd_num);
	}
	func(node->l_node, env, msh_fd);
}

void	exec_pipe(t_parser_node *node, t_env *env, t_info_fd *msh_fd)
{
	int		pipe_fd[2];
	int		status;
	pid_t	child_p1;
	pid_t	child_p2;

	if (node == NULL)
		return ;

	if (node->content->flag  == FT_COMMAND_F)
	{
		exec_command(node->content, env, 1);
		free_fd(&msh_fd);
		return ;
	}
	else if (node->content->flag == FT_REDIRECT_I_F
			|| node->content->flag == FT_REDIRECT_O_F
			|| node->content->flag == FT_REDIRECT_A_F)
	{
		exec_redirect(node, msh_fd, env, exec_pipe);
	}
	else if (node->content->flag == FT_PIPE_F)
	{
		pipe(pipe_fd);
		signal(SIGINT, sig_handler_c);
		signal(SIGQUIT, sig_handler_c);
		child_p1 = fork();
		if (child_p1 == 0)
		{
			close(pipe_fd[READ]);
			dup2(pipe_fd[WRITE], STDOUT_FILENO);
			close(pipe_fd[WRITE]);
			exec_pipe(node->l_node, env, msh_fd);
			exit(g_exit_status);
		}
		child_p2 = fork();
		if (child_p2 == 0)
		{
			close(pipe_fd[WRITE]);
			dup2(pipe_fd[READ], STDIN_FILENO);
			close(pipe_fd[READ]);
			exec_pipe(node->r_node, env, msh_fd);
			exit(g_exit_status);
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
		g_exit_status = get_exit_status(status);
	}
}

void	execute(t_parser_node *node, t_env *env, t_info_fd *msh_fd)
{
	if (node == NULL)
		return ;
	else if (node->content->flag == FT_SEMICOLON_F)
	{
		//printf("------------------------ : \n");
		execute(node->l_node, env, msh_fd);
		reset_fd(msh_fd);
		free_fd(&msh_fd);
		expansion(node->r_node, env);
		execute(node->r_node, env, msh_fd);
		//printf("g_exit_status : %d\n", *g_exit_status);
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
