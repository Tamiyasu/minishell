/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/06 16:10:41 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

// typedef struct	s_command
// {
// 	char	*command;
// 	char	**args;
// }				t_command;

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

void	init_pipe_fd(int *l_pipe_fd, int *r_pipe_fd, int *save_pipe_fd)
{
	l_pipe_fd[P_READ] = -1;
	l_pipe_fd[P_WRITE] = -1;
	r_pipe_fd[P_READ] = -1;
	r_pipe_fd[P_WRITE] = -1;
	save_pipe_fd[P_READ] = -1;
	save_pipe_fd[P_WRITE] = -1;
}

void	execute_2(t_parser_node *node, int *l_pipe_fd, int *r_pipe_fd, int *save_pipe_fd, t_env *env, int *exit_status)
{
	t_parser_node	*copy_node = node;
	int				pid_status;
	pid_t			child_p1;
	pid_t			child_p2;

	if (node == NULL)
	{
		return ;
	}
	if (node->content->flag == FT_SEMICOLON_F)
	{
		printf("node * : %p\n", node);
		printf("---in FT_SEMICOLON_F::node->content=[%s]\n1::in parent pid=[%d]\n", node->content->data, getpid());
		if (save_pipe_fd[P_READ])
		{
			close(save_pipe_fd[P_READ]);
			close(save_pipe_fd[P_WRITE]);
		}
		init_pipe_fd(l_pipe_fd, r_pipe_fd, save_pipe_fd);
		execute_2(node->l_node, l_pipe_fd, r_pipe_fd, save_pipe_fd, env, exit_status);
		execute_2(node->r_node, l_pipe_fd, r_pipe_fd, save_pipe_fd, env, exit_status);
	}
	else if (node->content->flag == FT_PIPE_F)
	{
		printf("---in FT_PIPE_F::node->content=[%s]\n1::in parent pid=[%d]\n", node->content->data, getpid());

		copy_node = node;

		//l_node
		node = node->l_node;
		if (node->content->flag != FT_COMMAND_F)
		{
			execute_2(node->l_node, l_pipe_fd, r_pipe_fd, save_pipe_fd, env, exit_status);
			execute_2(node->r_node, l_pipe_fd, r_pipe_fd, save_pipe_fd, env, exit_status);
		}
		if (node->content->flag == FT_COMMAND_F)
		{
			if (!(exec_check_builtin(node->content->data)))
			{
				//printf("l_node::in not builtin[%s]\n", node->content->data);
				exec_search_command_path(node->content, env);
				//printf("new command_path[%s]\n", node->content->data);
			}
			pipe(l_pipe_fd);
			child_p1 = fork();
			if  (child_p1 == 0)
			{
				if (save_pipe_fd[P_READ])
				{
					close(save_pipe_fd[P_WRITE]);
					dup2(save_pipe_fd[P_READ], 0);
					close(save_pipe_fd[P_READ]);
				}
				close(l_pipe_fd[P_READ]);
				dup2(l_pipe_fd[P_WRITE], 1);
				close(l_pipe_fd[P_WRITE]);

				exec_command(node->content, env, exit_status);
				exit(*exit_status);
			}
			if (save_pipe_fd[P_READ])
			{
				close(save_pipe_fd[P_READ]);
				close(save_pipe_fd[P_WRITE]);
			}
			save_pipe_fd[P_READ] = dup(l_pipe_fd[P_READ]);
			save_pipe_fd[P_WRITE] = dup(l_pipe_fd[P_WRITE]);
			close(l_pipe_fd[P_READ]);
			close(l_pipe_fd[P_WRITE]);
			waitpid(child_p1, &pid_status, 0);
			*exit_status = WEXITSTATUS(pid_status);
			printf("child1 command=[%s], end status=[%d][%d][%d][%d][%d][%d][%d][%d]\nsave_pipe_fd[READ]=[%d] save_pipe_fd[WRITE]=[%d]\n",  node->content->data, WIFEXITED(pid_status), WEXITSTATUS(pid_status), WIFSIGNALED(pid_status), WTERMSIG(pid_status), WCOREDUMP(pid_status),  WIFSTOPPED(pid_status), WSTOPSIG(pid_status), WIFCONTINUED(pid_status), save_pipe_fd[P_READ], save_pipe_fd[P_WRITE]);//del
		}
		// else//node->content->flag != COMMAND
		// {
		// 	//printf("-----node->content=[%s]\n1::in parent pid=[%d]\n", node->content->data, getpid());
		// 	execute_2(node->l_node, l_pipe_fd, r_pipe_fd, save_pipe_fd, env, exit_status);
		// }

		//r_node
		node = copy_node->r_node;
		if (node->content->flag == FT_COMMAND_F)
		{
			if (!(exec_check_builtin(node->content->data)))
			{
				//printf("r_node::in not builtin[%s]\n", node->content->data);
				exec_search_command_path(node->content, env);
				//printf("new command_path[%s]\n", node->content->data);
			}
			//pipe(r_pipe_fd);
			child_p2 = fork();
			if (child_p2 == 0)
			{
				if (save_pipe_fd[P_READ])
				{
					close(save_pipe_fd[P_WRITE]);
					dup2(save_pipe_fd[P_READ], 0);
					close(save_pipe_fd[P_READ]);
				}
				// close(r_pipe_fd[P_READ]);
				// dup2(r_pipe_fd[P_WRITE], 1);
				// close(r_pipe_fd[P_WRITE]);

				// printf("-------------------------in child2 pid=[%d] command=[%s]::pipe_READ[%d]::pipe_WRITE[%d]\n", getpid(), node->content->data, dup[0], dup[1]);

				exec_command(node->content, env, exit_status);
				exit(*exit_status);
			}
			if (save_pipe_fd[P_READ])
			{
				close(save_pipe_fd[P_READ]);
				close(save_pipe_fd[P_WRITE]);
			}
			save_pipe_fd[P_READ] = dup(r_pipe_fd[P_READ]);
			save_pipe_fd[P_WRITE] = dup(r_pipe_fd[P_WRITE]);
			close(r_pipe_fd[P_WRITE]);
			close(r_pipe_fd[P_READ]);
			waitpid(child_p2, &pid_status, 0);
			*exit_status = WEXITSTATUS(pid_status);
			// printf("the child2 pid_status=[%d][%d][%d][%d][%d][%d][%d][%d]\n",  WIFEXITED(pid_status), WEXITSTATUS(pid_status), WIFSIGNALED(pid_status), WTERMSIG(pid_status), WCOREDUMP(pid_status),  WIFSTOPPED(pid_status), WSTOPSIG(pid_status), WIFCONTINUED(pid_status));//del
			printf("child2 command=[%s] end status=[%d][%d][%d][%d][%d][%d][%d][%d]\nsave_pipe_fd[READ]=[%d] save_pipe_fd[WRITE]=[%d]\n", node->content->data, WIFEXITED(pid_status), WEXITSTATUS(pid_status), WIFSIGNALED(pid_status), WTERMSIG(pid_status), WCOREDUMP(pid_status),  WIFSTOPPED(pid_status), WSTOPSIG(pid_status), WIFCONTINUED(pid_status), save_pipe_fd[P_READ], save_pipe_fd[P_WRITE]);//del
		}
		// else//node->content->flag != COMMAND
		// {
		// 	execute_2(node->l_node, l_pipe_fd, r_pipe_fd, save_pipe_fd, env, exit_status);
		// }
		// printf("2::in parent pid=[%d]\n", getpid());
	}
	else if(node->content->flag == FT_COMMAND_F)
	{
		//printf("---in FT_COMMAND_F::node->content=[%s]\n1::in parent pid=[%d]\n", node->content->data, getpid());
		if (exec_check_builtin(node->content->data))
		{
			exec_command(node->content, env, exit_status);
		}
		else
		{
			//printf("in search_comman_path[%s]\n", node->content->data);
			exec_search_command_path(node->content, env);
			//printf("out search_comman_path[%s]\n", node->content->data);
			child_p1 = fork();
			if (child_p1 == 0)
			{
				execute_execve(node->content, env);
			}
			waitpid(child_p1, &pid_status, 0);
			*exit_status = WEXITSTATUS(pid_status);
		}
	}

	close(save_pipe_fd[P_READ]);
	close(save_pipe_fd[P_WRITE]);

	printf("------------------------------in parent[%s]::pipe_READ[%d]::pipe_WRITE[%d]\n", node->content->data, dup(0), dup(1));

	// close(save_pipe_fd[P_WRITE]);
	// dup2(save_pipe_fd[P_READ], 0);
	// close(save_pipe_fd[P_READ]);

	// *exit_status = WEXITSTATUS(pid_status);

}

void	execute(t_parser_node *node, t_env *env, int *exit_status)
{
	// t_parser_node	*copy_node = node;
	// int				l_pipe_fd[2];
	// int				r_pipe_fd[2];
	// int				save_pipe_fd[2];
	int	pipe_fd[2];

	int				pid_status;
	pid_t			child_p1;
	pid_t			child_p2;

	// int	save_stdin = dup(0);
	// int save_stdout = dup(1);

	printf("---in exexute----------------------------------\n");
	// init_pipe_fd(l_pipe_fd, r_pipe_fd, save_pipe_fd);
	// execute_2(node, l_pipe_fd, r_pipe_fd, save_pipe_fd, env, exit_status);
	//execute_2(node, env, exit_status);
	// l_pipe_fd[P_READ] = -1;
	// l_pipe_fd[P_WRITE] = -1;
	// r_pipe_fd[P_READ] = -1;
	// r_pipe_fd[P_WRITE] = -1;
	// save_pipe_fd[P_READ] = -1;
	// save_pipe_fd[P_WRITE] = -1;

	// //if (node == NULL)
	// // {
	// // 	return ;
	// // }

	// if (node->content->flag == FT_SEMICOLON_F)
	// {
	// 	init_pipe_fd(l_pipe_fd, r_pipe_fd, save_pipe_fd);
	// 	close(save_pipe_fd[P_READ]);
	// 	close(save_pipe_fd[P_WRITE]);
	// 	execute(node->l_node, env, exit_status);
	// 	execute(node->r_node, env, exit_status);
	// }
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
				//printf("l_node::in not builtin[%s]\n", node->content->data);
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
				//printf("l_node::in not builtin[%s]\n", node->content->data);
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
	// waitpid(child_p1, &pid_status, 0);
	// waitpid(child_p2, &pid_status, 0);
	// close(pipe_fd[P_READ]);
	// close(pipe_fd[P_WRITE]);

	// 	copy_node = node;

	// 	//l_node
	// 	node = node->l_node;
	// 	if (node->content->flag == FT_COMMAND_F)
	// 	{
	// 		if (!(exec_check_builtin(node->content->data)))
	// 		{
	// 			exec_search_command_path(node->content, env);
	// 		}
	// 		pipe(l_pipe_fd);
	// 		child_p1 = fork();
	// 		if  (child_p1 == 0)
	// 		{
	// 			printf("------------------------------in child1[%s]\n", node->content->data);

	// 			if (save_pipe_fd[P_READ])
	// 			{
	// 				close(save_pipe_fd[P_WRITE]);
	// 				dup2(save_pipe_fd[P_READ], 0);
	// 				close(save_pipe_fd[P_READ]);
	// 			}
	// 			close(l_pipe_fd[P_READ]);
	// 			dup2(l_pipe_fd[P_WRITE], 1);
	// 			close(l_pipe_fd[P_WRITE]);
	// 			exec_command(node->content, env, exit_status);
	// 			exit(*exit_status);
	// 		}
	// 		if (save_pipe_fd[P_READ])
	// 		{
	// 			close(save_pipe_fd[P_READ]);
	// 			close(save_pipe_fd[P_WRITE]);
	// 		}
	// 		save_pipe_fd[P_READ] = dup(l_pipe_fd[P_READ]);
	// 		save_pipe_fd[P_WRITE] = dup(l_pipe_fd[P_WRITE]);
	// 		close(l_pipe_fd[P_READ]);
	// 		close(l_pipe_fd[P_WRITE]);
	// 		waitpid(child_p1, &pid_status, 0);
	// 		printf("the child1 pid=[%d]::pid_status=[%d][%d][%d][%d][%d][%d][%d][%d]\n", child_p1, WIFEXITED(pid_status), WEXITSTATUS(pid_status), WIFSIGNALED(pid_status), WTERMSIG(pid_status), WCOREDUMP(pid_status),  WIFSTOPPED(pid_status), WSTOPSIG(pid_status), WIFCONTINUED(pid_status));//del
	// 	}
	// 	else//node->content->flag != COMMAND
	// 	{
	// 		execute(node->l_node, env, *exit_status);
	// 	}
	// 	//r_node
	// 	node = copy_node->r_node;
	// 	if (node->content->flag == FT_COMMAND_F)
	// 	{
	// 		if (!(exec_check_builtin(node->content->data)))
	// 		{
	// 			exec_search_command_path(node->content, env);
	// 		}
	// 		pipe(r_pipe_fd);
	// 		child_p2 = fork();
	// 		if (child_p2 == 0)
	// 		{
	// 			printf("------------------------------in child2[%s]\n", node->content->data);

	// 			if (save_pipe_fd[P_READ])
	// 			{
	// 				close(save_pipe_fd[P_WRITE]);
	// 				dup2(save_pipe_fd[P_READ], 0);
	// 				close(save_pipe_fd[P_READ]);
	// 			}
	// 			close(r_pipe_fd[P_READ]);
	// 			dup2(r_pipe_fd[P_WRITE], 1);
	// 			close(r_pipe_fd[P_WRITE]);
	// 			exec_command(node->content, env, exit_status);
	// 			exit(*exit_status);
	// 		}
	// 		if (save_pipe_fd[P_READ])
	// 		{
	// 			close(save_pipe_fd[P_READ]);
	// 			close(save_pipe_fd[P_WRITE]);
	// 		}
	// 		save_pipe_fd[P_READ] = dup(r_pipe_fd[P_READ]);
	// 		save_pipe_fd[P_WRITE] = dup(r_pipe_fd[P_WRITE]);
	// 		close(r_pipe_fd[P_WRITE]);
	// 		close(r_pipe_fd[P_READ]);
	// 		waitpid(child_p2, &pid_status, 0);
	// 		printf("the child2 pid=[%d]::pid_status=[%d][%d][%d][%d][%d][%d][%d][%d]\n", child_p2, WIFEXITED(pid_status), WEXITSTATUS(pid_status), WIFSIGNALED(pid_status), WTERMSIG(pid_status), WCOREDUMP(pid_status),  WIFSTOPPED(pid_status), WSTOPSIG(pid_status), WIFCONTINUED(pid_status));//del
	// 	}
	// 	else//node->content->flag != COMMAND
	// 	{
	// 		execute(node->r_node, env, *exit_status);
	// 	}
	// 	printf("2::in parent pid=[%d]\n", getpid());
	// }

	// else if(node->content->flag == FT_COMMAND_F)
	// {
	// 	if (exec_check_builtin(node->content->data))
	// 	{
	// 		exec_command(node->content, env, exit_status);
	// 	}
	// 	else
	// 	{
	// 		printf("in search_comman_path[%s]\n", node->content->data);
	// 		exec_search_command_path(node->content, env);
	// 		printf("out search_comman_path[%s]\n", node->content->data);
	// 		child_p1 = fork();
	// 		if (child_p1 == 0)
	// 		{
	// 			execute_execve(node->content, env);
	// 		}
	// 		waitpid(child_p1, &pid_status, 0);
	// 		*exit_status = WEXITSTATUS(pid_status);
	// 	}
	// }

	// save_pipe_fd[P_READ] = dup(r_pipe_fd[P_READ]);
	// save_pipe_fd[P_WRITE] = dup(r_pipe_fd[P_WRITE]);
	// close(r_pipe_fd[P_WRITE]);
	// close(r_pipe_fd[P_READ]);

	// close(save_pipe_fd[P_WRITE]);
	// dup2(save_pipe_fd[P_READ], 0);
	// close(save_pipe_fd[P_READ]);

	// *exit_status = WEXITSTATUS(pid_status);
	// printf("end all child process:exit status=[%d]\n", *exit_status);
	printf("---exec end------------------------\n");
}
