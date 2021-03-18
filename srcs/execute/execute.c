/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/18 18:20:21 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "signal_handler.h"

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

void	command_builtin(t_token *token, t_env *env, int *exit_status)
{
	if (ft_strcmp(token->data, "cd") == 0)
		*exit_status = execute_cd(token, env);
	else if (ft_strcmp(token->data, "echo") == 0)
		*exit_status = execute_echo(token);
	else if (ft_strcmp(token->data, "env") == 0)
		*exit_status = execute_env(env->data);
	else if (ft_strcmp(token->data, "export") == 0)
		*exit_status = execute_export(token, env);
	else if (ft_strcmp(token->data, "pwd") == 0)
		*exit_status = execute_pwd();
	else if (ft_strcmp(token->data, "unset") == 0)
		*exit_status = execute_unset(token, env);
	else if (ft_strcmp(token->data, "exit") == 0)
		execute_exit(token, exit_status);
	return ;
}

int	 redirect_check_fdnum(char *data, int redirect_flag)
{
	int	fd_num;

	fd_num = ft_atoi(data);
	if (fd_num == 0 && redirect_flag == FT_REDIRECT_O_F)
	{
		fd_num = 1;
	}
	return (fd_num);
}

void	exec_command(t_token *token, t_env *env, int *exit_status, int child_flag)
{
	pid_t	child_p;
	int		pid_status;

	pid_status = 0;
	if (ft_strcmp(token->data, "") == 0)
		return ;
	if (exec_check_builtin(token->data))
	{
		command_builtin(token, env, exit_status);
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

void	exec_redirect(t_parser_node *node, t_info_fd *msh_fd,
						t_env *env, int *exit_status,
						void (*func)(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *msh_fd))
{
	int	fd_num;
	int open_fd;

	fd_num = redirect_check_fdnum(node->content->data, node->content->flag);
	open_fd = redirect_file_open(node->r_node->content->data, node->content->flag);
	if (open_fd == -1)
	{
		*exit_status = 1;
		return ;
	}
	if (redirect_check_reserve(msh_fd, fd_num, node->content->flag))
	{
		msh_fd = redirect_save_fd(msh_fd, fd_num, node->content->flag);
		dup2(open_fd, fd_num);
	}
	//close(open_fd);
	func(node->l_node, env, exit_status, msh_fd);
}

void	exec_pipe(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *msh_fd)
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
		free_fd(&msh_fd);
		return ;
	}
	else if (node->content->flag == FT_REDIRECT_I_F
			|| node->content->flag == FT_REDIRECT_O_F
			|| node->content->flag == FT_REDIRECT_A_F)
	{
		exec_redirect(node, msh_fd, env, exit_status, exec_pipe);
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
			exec_pipe(node->l_node, env, exit_status, msh_fd);
			exit(*exit_status);
		}
		child_p2 = fork();
		if (child_p2 == 0)
		{
			close(pipe_fd[WRITE]);
			dup2(pipe_fd[READ], STDIN_FILENO);
			close(pipe_fd[READ]);
			exec_pipe(node->r_node, env, exit_status, msh_fd);
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

void	execute(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *msh_fd)
{
	if (node == NULL)
		return ;
	else if (node->content->flag == FT_SEMICOLON_F)
	{
		//printf("------------------------ : \n");
		execute(node->l_node, env, exit_status, msh_fd);
		reset_fd(msh_fd);
		free_fd(&msh_fd);
		execute(node->r_node, env, exit_status, msh_fd);
		//printf("exit_status : %d\n", *exit_status);
	}
	else if (node->content->flag == FT_PIPE_F)
		exec_pipe(node, env, exit_status, msh_fd);
	else if (node->content->flag == FT_REDIRECT_I_F
			|| node->content->flag == FT_REDIRECT_O_F
			|| node->content->flag == FT_REDIRECT_A_F)
	{
		exec_redirect(node, msh_fd, env, exit_status, execute);
	}
	else if (node->content->flag == FT_COMMAND_F)
	{
		exec_command(node->content, env, exit_status, 0);
		reset_fd(msh_fd);
		free_fd(&msh_fd);
	}
}
