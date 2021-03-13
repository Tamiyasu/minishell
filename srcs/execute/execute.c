/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/13 18:43:56 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys/types.h"
#include "sys/stat.h"
#include "execute.h"
#include "libft.h"
#include "signal_handler.h"

void	init_fd(t_info_fd *fd)
{
	fd->redirect_i = -1;
	fd->redirect_o = -1;
	fd->save_stdin = -1;
	fd->save_stdout = -1;
	fd->fd_num = -1;
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

void	exec_search_command_path(t_lsttoken *token, t_env *env)
{
	char			**path_value;
	int				idx;
	DIR				*dp;
	struct dirent	*dirp;
	char			*tmp;


	if (token->data[0] == '.' || token->data[0] == '/')
	{
		return ;
	}
	idx = msh_env_search(env->data, "PATH");
	path_value = ft_split(&env->data[idx][5], ':');
	idx= 0;
	while (path_value[idx] != NULL)
	{
		dp = opendir(path_value[idx]);
		if (dp == NULL)
		{
			ft_putendl_fd(strerror(errno), 1);
			return ;
		}
		while ((dirp = readdir(dp)) != NULL)
		{
			if (ft_strcmp(token->data, dirp->d_name) == 0)
			{
				tmp = ft_strjoin("/", token->data);
				free(token->data);
				token->data = ft_strjoin(path_value[idx],  tmp);
				free(tmp);
				tmp = NULL;
				closedir(dp);
				free_args(path_value);
				return ;
			}
		}
		closedir(dp);
		idx++;
	}
	free_args(path_value);
}

/*
** コマンドを実行する　returnで終了ステータスを返す.
*/
void	exec_command(t_lsttoken *token, t_env *env, int *exit_status)
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
	else
	{
		execute_execve(token, env);
	}
	return ;
}

void	redirect_exec_dup(t_info_fd *fd)
{
	if (fd->redirect_i != -1)
	{
		dup2(fd->redirect_i, STDIN_FILENO);
		close(fd->redirect_i);
	}
	if (fd->redirect_o != -1)
	{
		dup2(fd->redirect_o, fd->fd_num);
		close(fd->redirect_o);
	}
}

void	redirect_check_fdnum(t_info_fd *fd, char *data)
{
		fd->fd_num = ft_atoi(data);
		if (fd->fd_num == 0)
		{
			fd->fd_num = 1;
		}
}

void	redirect_save_fd(t_info_fd *fd, int dup_fd, int open_fd)
{
	if (dup_fd == STDIN_FILENO)
	{
		if (fd->save_stdin == -1)
		{
			fd->save_stdin = dup(dup_fd);
			fd->redirect_i = open_fd;
		}
	}
	else
	{
		if (fd->save_stdout == -1)
		{
			fd->save_stdout = dup(dup_fd);
			fd->redirect_o = open_fd;
		}
	}
}

void	redirect_close_fd(int save_fd, int close_fd)
{
	if (save_fd != -1)
	{
		dup2(save_fd, close_fd);
		close(save_fd);
	}
}

void	exec_simple_command(t_parser_node *node, t_info_fd *fd, t_env *env, int *exit_status)
{
	//pid_t	child_p;
	int		pid_status;

	redirect_exec_dup(fd);
	if (exec_check_builtin(node->content->data))
	{
		exec_command(node->content, env, exit_status);
		return ;
	}
	exec_search_command_path(node->content, env);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
	c_pid(fork());
	if (c_pid(-1) == 0)
	{
		signal(SIGINT, sig_handler_c);
		signal(SIGQUIT, sig_handler_c);
		execute_execve(node->content,env);
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
}

void	exec_command_in_childp(t_lsttoken *token, t_env *env, int *exit_status)
{
	if (!(exec_check_builtin(token->data)))
	{
		exec_search_command_path(token, env);
	}
	exec_command(token, env, exit_status);
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
		redirect_exec_dup(fd);
		exec_command_in_childp(node->content, env, exit_status);
	}
	else if (node->content->flag == FT_REDIRECT_I_F)
	{
		if ((open_fd = redirect_in_check_open(node->r_node->content->data))== -1)
			return ;
		fd->redirect_i = open_fd;
		exec_pipe(node->l_node, env, exit_status, fd);
		close(fd->redirect_i);
	}
	else if (node->content->flag == FT_REDIRECT_O_F || node->content->flag == FT_REDIRECT_A_F)
	{
		redirect_check_fdnum(fd, node->content->data);
		if ((open_fd = redirect_out_check_open(node->r_node->content->data, node->content->flag)) == -1)
			return ;
		fd->redirect_o = open_fd;
		exec_pipe(node->l_node, env, exit_status, fd);
		close(fd->redirect_o);
	}
	else if (node->content->flag == FT_PIPE_F)
	{
		pipe(pipe_fd);
		child_p1 = fork();
		if (child_p1 == 0)
		{
			close(pipe_fd[READ]);
			dup2(pipe_fd[WRITE], 1);
			close(pipe_fd[WRITE]);
			exec_pipe(node->l_node, env, exit_status, fd);
			redirect_exec_dup(fd);
			exec_command_in_childp(node->content, env, exit_status);
			exit(*exit_status);
		}
		child_p2 = fork();
		if (child_p2 == 0)
		{
			close(pipe_fd[WRITE]);
			dup2(pipe_fd[READ], 0);
			close(pipe_fd[READ]);
			if (node->r_node->content->flag != FT_COMMAND_F)
			{
				exec_pipe(node->r_node, env, exit_status, fd);
				//return ;
			}
			exec_command_in_childp(node->r_node->content, env, exit_status);
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
	int			open_fd;

	if (node == NULL)
		return ;
	if (node->content->flag == FT_SEMICOLON_F)
	{
		execute(node->l_node, env, exit_status, fd);
		init_fd(fd);
		execute(node->r_node, env, exit_status, fd);
	}
	else if (node->content->flag == FT_PIPE_F)
		exec_pipe(node, env, exit_status, fd);
	else if (node->content->flag == FT_REDIRECT_I_F)
	{
		if ((open_fd = redirect_in_check_open(node->r_node->content->data)) == -1)
			return ;
		redirect_save_fd(fd, STDIN_FILENO, open_fd);
		execute(node->l_node, env, exit_status, fd);
		redirect_close_fd(fd->save_stdin, STDIN_FILENO);
	}
	else if (node->content->flag == FT_REDIRECT_O_F || node->content->flag == FT_REDIRECT_A_F)
	{
		redirect_check_fdnum(fd, node->content->data);
		if ((open_fd = redirect_out_check_open(node->r_node->content->data, node->content->flag)) == -1)
			return ;
		redirect_save_fd(fd, fd->fd_num, open_fd);
		execute(node->l_node, env, exit_status, fd);
		redirect_close_fd(fd->save_stdout, fd->fd_num);
	}
	if (node->content->flag == FT_COMMAND_F)
		exec_simple_command(node, fd, env, exit_status);
}
