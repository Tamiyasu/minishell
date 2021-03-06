/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/06 17:31:18 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

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
		printf("in no search[%c]\n", token->data[0]);
		return ;
	}
	idx = msh_env_search(env->data, "PATH");
	path_value = ft_split(&env->data[idx][5], ':');
	idx= 0;
	for (int x = 0; path_value[x] != NULL; x++)
	{
		printf("path_value[%d]=%s\n", x, path_value[x]);
	}
	while (path_value[idx] != NULL)
	{
		//printf("path_value[%d]=[%s]\n", idx, path_value[idx]);//del
		dp = opendir(path_value[idx]);
		if (dp == NULL)
		{
			ft_putendl_fd(strerror(errno), 1);
			return ;
		}
		while ((dirp = readdir(dp)) != NULL)
		{
			//printf("file name[%s]\n", dirp->d_name);//del
			if (ft_strcmp(token->data, dirp->d_name) == 0)
			{
				//printf("file name[%s]\n", dirp->d_name);//del
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

void	exec_pipe(t_parser_node *node, t_env *env, int *exit_status)
{
	int	pipe_fd[2];

	int				pid_status;
	pid_t			child_p1;
	pid_t			child_p2;

	printf("---in exec_pipe----------------------------------\n");

	if (node == NULL)
	{
		return ;
	}
	if (node->content->flag  == FT_COMMAND_F)
	{
		if (!(exec_check_builtin(node->content->data)))
		{
			//printf("l_node::in not builtin[%s]\n", node->content->data);
			exec_search_command_path(node->content, env);
			printf("new command_path[%s]\n\n", node->content->data);
		}
		//printf("command[%s], STDIN[%d], STDOUT[%d]\n", node->content->data, dup(0), dup(1));
		exec_command(node->content, env, exit_status);
	}
	else if (node->content->flag == FT_PIPE_F)
	{
		printf("root_node->content=[%s], node->l_node->content[%s], node->r_node->content[%s]\n\n", node->content->data, node->l_node->content->data, node->r_node->content->data);

		pipe(pipe_fd);

		child_p1 = fork();
		if (child_p1 == 0)
		{
			close(pipe_fd[P_READ]);
			dup2(pipe_fd[P_WRITE], 1);
			close(pipe_fd[P_WRITE]);

			execute(node->l_node, env, exit_status);
			if (!(exec_check_builtin(node->content->data)))
			{
				exec_search_command_path(node->content, env);
				printf("new command_path[%s]\n\n", node->content->data);
			}
			printf("command[%s], STDIN[%d], STDOUT[%d]\n\n", node->content->data, dup(0), dup(1));
			exec_command(node->content, env, exit_status);
			exit(*exit_status);
		}
		child_p2 = fork();
		if (child_p2 == 0)
		{
			close(pipe_fd[P_WRITE]);
			dup2(pipe_fd[P_READ], 0);
			close(pipe_fd[P_READ]);

			if (!(exec_check_builtin(node->r_node->content->data)))
			{
				exec_search_command_path(node->r_node->content, env);
				printf("new command_path[%s]\n\n", node->r_node->content->data);
			}
			printf("command[%s], STDIN[%d], STDOUT[%d]\n------------------------------------\n", node->r_node->content->data, dup(0), dup(1));
			exec_command(node->r_node->content, env, exit_status);
			exit(*exit_status);
		}
		close(pipe_fd[P_READ]);
		close(pipe_fd[P_WRITE]);
		waitpid(child_p1, &pid_status, 0);
		waitpid(child_p2, &pid_status, 0);
		*exit_status = WEXITSTATUS(pid_status);
	}
	else if (node->content->flag == FT_SEMICOLON_F)
	{
		execute(node->l_node, env, exit_status);
		execute(node->r_node, env, exit_status);
	}

	printf("---exec_pipe end------------------------\n");
}

void	execute(t_parser_node *node, t_env *env, int *exit_status)
{
	int	child_p;
	int	pid_status;

	if (node == NULL)
		return ;
	if (node->content->flag == FT_COMMAND_F)
	{
		if (exec_check_builtin(node->content->data))
		{
			exec_command(node->content, env, exit_status);
		}
		else
		{
			exec_search_command_path(node->content, env);
			child_p = fork();
			if (child_p == 0)
			{
				execute_execve(node->content,env);
			}
			waitpid(child_p, &pid_status, 0);
			*exit_status = WEXITSTATUS(pid_status);
		}
	}
	else if (node->content->flag == FT_SEMICOLON_F)
	{
		execute(node->l_node, env, exit_status);
		execute(node->r_node, env, exit_status);
	}
	else if (node->content->flag == FT_PIPE_F)
	{
		exec_pipe(node, env, exit_status);
	}
	// int	pipe_fd[2];

	// int				pid_status;
	// pid_t			child_p1;
	// pid_t			child_p2;

	// printf("---in exexute----------------------------------\n");

	// if (node == NULL)
	// {
	// 	return ;
	// }
	// if (node->content->flag  == FT_COMMAND_F)
	// {
	// 	if (!(exec_check_builtin(node->content->data)))
	// 	{
	// 		//printf("l_node::in not builtin[%s]\n", node->content->data);
	// 		exec_search_command_path(node->content, env);
	// 		printf("new command_path[%s]\n\n", node->content->data);
	// 	}
	// 	//printf("command[%s], STDIN[%d], STDOUT[%d]\n", node->content->data, dup(0), dup(1));
	// 	exec_command(node->content, env, exit_status);
	// }
	// else if (node->content->flag == FT_PIPE_F)
	// {
	// 	printf("root_node->content=[%s], node->l_node->content[%s], node->r_node->content[%s]\n\n", node->content->data, node->l_node->content->data, node->r_node->content->data);

	// 	pipe(pipe_fd);

	// 	child_p1 = fork();
	// 	if (child_p1 == 0)
	// 	{
	// 		close(pipe_fd[P_READ]);
	// 		dup2(pipe_fd[P_WRITE], 1);
	// 		close(pipe_fd[P_WRITE]);

	// 		execute(node->l_node, env, exit_status);
	// 		if (!(exec_check_builtin(node->content->data)))
	// 		{
	// 			exec_search_command_path(node->content, env);
	// 			printf("new command_path[%s]\n\n", node->content->data);
	// 		}
	// 		printf("command[%s], STDIN[%d], STDOUT[%d]\n\n", node->content->data, dup(0), dup(1));
	// 		exec_command(node->content, env, exit_status);
	// 		exit(*exit_status);
	// 	}
	// 	child_p2 = fork();
	// 	if (child_p2 == 0)
	// 	{
	// 		close(pipe_fd[P_WRITE]);
	// 		dup2(pipe_fd[P_READ], 0);
	// 		close(pipe_fd[P_READ]);

	// 		if (!(exec_check_builtin(node->r_node->content->data)))
	// 		{
	// 			exec_search_command_path(node->r_node->content, env);
	// 			printf("new command_path[%s]\n\n", node->r_node->content->data);
	// 		}
	// 		printf("command[%s], STDIN[%d], STDOUT[%d]\n------------------------------------\n", node->r_node->content->data, dup(0), dup(1));
	// 		exec_command(node->r_node->content, env, exit_status);
	// 		exit(*exit_status);
	// 	}
	// 	close(pipe_fd[P_READ]);
	// 	close(pipe_fd[P_WRITE]);
	// 	waitpid(child_p1, &pid_status, 0);
	// 	waitpid(child_p2, &pid_status, 0);
	// 	*exit_status = WEXITSTATUS(pid_status);
	// }
	// else if (node->content->flag == FT_SEMICOLON_F)
	// {
	// 	execute(node->l_node, env, exit_status);
	// 	execute(node->r_node, env, exit_status);
	// }

	// printf("---exec end------------------------\n");
}
