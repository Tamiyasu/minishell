/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/09 18:01:24 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sys/types.h"
#include "sys/stat.h"
#include "execute.h"
#include "libft.h"

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

void	exec_dup_redirect(t_info_fd *fd)
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

void	exec_simple_command(t_parser_node *node, t_info_fd *fd, t_env *env, int *exit_status)
{
	pid_t	child_p;
	int		pid_status;

	exec_dup_redirect(fd);
	if (exec_check_builtin(node->content->data))
	{
		exec_command(node->content, env, exit_status);
		return ;
	}
	exec_search_command_path(node->content, env);
	child_p = fork();
	if (child_p == 0)
		execute_execve(node->content,env);
	waitpid(child_p, &pid_status, 0);
	*exit_status = WEXITSTATUS(pid_status);
}

void	exec_pipe(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *fd)
{
	int		pipe_fd[2];
	int		open_fd;
	int		status;
	pid_t	child_p1;
	pid_t	child_p2;

	printf("---in exec_pipe----------------------------------\n");

	if (node == NULL)
		return ;

	if (node->content->flag == FT_REDIRECT_I_F)
	{
		open_fd = open(node->r_node->content->data, O_RDONLY);
		if (!open_fd)
		{
			ft_putendl_fd(strerror(errno), STDOUT_FILENO);
			return ;
		}
		if (fd->save_stdin == -1)
		{
			fd->save_stdin = dup(STDIN_FILENO);
			fd->redirect_i = open_fd;
		}
		exec_pipe(node->l_node, env, exit_status, fd);
		if (fd->save_stdin != -1)
		{
			dup2(fd->save_stdin, STDIN_FILENO);
			close(fd->save_stdin);
		}
	}
	else if (node->content->flag == FT_REDIRECT_O_F)
	{
		fd->fd_num = ft_atoi(node->content->data);
		if (fd->fd_num == 0)
		{
			fd->fd_num = 1;
		}
		open_fd = open(node->r_node->content->data, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (!open_fd)
		{
			ft_putendl_fd(strerror(errno), STDOUT_FILENO);
			return ;
		}
		if (fd->save_stdout == -1)
		{
			fd->save_stdout = dup(fd->fd_num);
			fd->redirect_o = open_fd;
		}
		exec_pipe(node->l_node, env, exit_status, fd);
		if (fd->save_stdout != -1)
		{
			dup2(fd->save_stdout, fd->fd_num);
			close(fd->save_stdout);
		}
	}
	else if (node->content->flag == FT_REDIRECT_A_F)
	{
		fd->fd_num = ft_atoi(node->content->data);
		if (fd->fd_num == 0)
		{
			fd->fd_num = 1;
		}
		open_fd = open(node->r_node->content->data, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (!open_fd)
		{
			ft_putendl_fd(strerror(errno), STDOUT_FILENO);
			return ;
		}
		if (fd->save_stdout == -1)
		{
			fd->save_stdout = dup(fd->fd_num);
			fd->redirect_o = open_fd;
		}
		exec_pipe(node->l_node, env, exit_status, fd);
		close(fd->redirect_o);
		if (fd->save_stdout != -1)
		{
			dup2(fd->save_stdout, fd->fd_num);
			close(fd->save_stdout);
		}
	}

	if (node->content->flag  == FT_COMMAND_F)
	{
		exec_dup_redirect(fd);
		if (!(exec_check_builtin(node->content->data)))
		{
			exec_search_command_path(node->content, env);
		}
		exec_command(node->content, env, exit_status);
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
			if (!(exec_check_builtin(node->content->data)))
			{
				exec_search_command_path(node->content, env);
			}
			exec_command(node->content, env, exit_status);
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
				return  ;
			}
			if (!(exec_check_builtin(node->r_node->content->data)))
			{
				exec_search_command_path(node->r_node->content, env);
			}
			exec_command(node->r_node->content, env, exit_status);
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
		open_fd = open(node->r_node->content->data, O_RDONLY);
		if (!open_fd)
		{
			ft_putendl_fd(strerror(errno), STDOUT_FILENO);
			return ;
		}
		if (fd->save_stdin == -1)
		{
			fd->save_stdin = dup(STDIN_FILENO);
			fd->redirect_i = open_fd;
		}
		execute(node->l_node, env, exit_status, fd);
		if (fd->save_stdin != -1)
		{
			dup2(fd->save_stdin, STDIN_FILENO);
			close(fd->save_stdin);
		}
	}
	else if (node->content->flag == FT_REDIRECT_O_F)
	{
		fd->fd_num = ft_atoi(node->content->data);
		if (fd->fd_num == 0)
		{
			fd->fd_num = 1;
		}
		open_fd = open(node->r_node->content->data, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (!open_fd)
		{
			ft_putendl_fd(strerror(errno), STDOUT_FILENO);
			return ;
		}
		if (fd->save_stdout == -1)
		{
			fd->save_stdout = dup(fd->fd_num);
			fd->redirect_o = open_fd;
		}
		execute(node->l_node, env, exit_status, fd);
		if (fd->save_stdout != -1)
		{
			dup2(fd->save_stdout, fd->fd_num);
			close(fd->save_stdout);
		}
	}
	else if (node->content->flag == FT_REDIRECT_A_F)
	{
		fd->fd_num = ft_atoi(node->content->data);
		if (fd->fd_num == 0)
		{
			fd->fd_num = 1;
		}
		open_fd = open(node->r_node->content->data, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (!open_fd)
		{
			ft_putendl_fd(strerror(errno), STDOUT_FILENO);
			return ;
		}
		if (fd->save_stdout == -1)
		{
			fd->save_stdout = dup(fd->fd_num);
			fd->redirect_o = open_fd;
		}
		execute(node->l_node, env, exit_status, fd);
		close(fd->redirect_o);
		if (fd->save_stdout != -1)
		{
			dup2(fd->save_stdout, fd->fd_num);
			close(fd->save_stdout);
		}
	}
	if (node->content->flag == FT_COMMAND_F)
	{
		exec_simple_command(node, fd, env, exit_status);
	}
}
