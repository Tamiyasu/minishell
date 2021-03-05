/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/05 15:21:57 by ysaito           ###   ########.fr       */
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
	//envからPATH変数のvariableを取得
		//while (path_variable[idx] != NULL)
			//opendir(path_variable[idx]);
				//if (token->data == derent->d_name)//ディレクトリ内に一致するファイル名あったら
					//closedir;
					//break;
				//derent = derent->next;
			//closedir();
			//idx++;
		//token->data[0] = ft_strjoin(path_variable[idx], token->data;

	char	**path_value;
	int		idx;
	DIR		*dp;
	struct dirent	*dirp;
	char	*tmp;


	if (token->data[0] == '.' || token->data[0] == '/')
	{
		return ;
	}
	idx = msh_env_search(env->data, "PATH");//この関数、returnをchar *値にする
	path_value = ft_split(&env->data[idx][5], ':');
	idx= 0;
	while (path_value[idx] != NULL)
	{
		printf("path_value[%d]=[%s]\n", idx, path_value[idx]);
		dp = opendir(path_value[idx]);
		if (dp == NULL)
		{
			ft_putendl_fd(strerror(errno), 1);
			return ;
		}
		while ((dirp = readdir(dp)) != NULL)
		{
			printf("file name[%s]\n", dirp->d_name);
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

void	execute(t_parser_node *node, t_env *env, int *exit_status)
{
	t_parser_node	*copy_node = node;
	int				l_pipe_fd[2];
	int				r_pipe_fd[2];
	int				save_pipe_fd[2];
	int				pid_status;
	pid_t			child_p1;
	pid_t			child_p2;

	//printf("---in exexute.c----------------------------------\n");
	l_pipe_fd[P_READ] = -1;
	l_pipe_fd[P_WRITE] = -1;
	r_pipe_fd[P_READ] = -1;
	r_pipe_fd[P_WRITE] = -1;
	save_pipe_fd[P_READ] = -1;
	save_pipe_fd[P_WRITE] = -1;

	// if (!(exec_check_builtin(node->content->data)))
	// {
	// 	exec_search_command_path(node->content, env);
	// }
	if(node->content->flag == FT_COMMAND_F)
	{
		if (!(exec_check_builtin(node->content->data)))
		{
			printf("in search_comman_path[%s]\n", node->content->data);
			exec_search_command_path(node->content, env);
			printf("out search_comman_path[%s]\n", node->content->data);
		}
	}


	if (node->content->flag == FT_PIPE_F)
	{
		printf("root->node->content=[%s]\n1::in parent pid=[%d]\n", node->content->data, getpid());

		copy_node = node;

		//l_node
		node = node->l_node;
		if (node->content->flag == FT_COMMAND_F)//以下左枝を実行
		{
			pipe(l_pipe_fd);
			child_p1 = fork();
			if  (child_p1 == 0)
			{
				printf("------------------------------in child1[%s]\n", node->content->data);

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
			printf("the child1 pid=[%d]::pid_status=[%d][%d][%d][%d][%d][%d][%d][%d]\n", child_p1, WIFEXITED(pid_status), WEXITSTATUS(pid_status), WIFSIGNALED(pid_status), WTERMSIG(pid_status), WCOREDUMP(pid_status),  WIFSTOPPED(pid_status), WSTOPSIG(pid_status), WIFCONTINUED(pid_status));//del
		}
		//r_node
		node = copy_node->r_node;
		if (node->content->flag == FT_COMMAND_F)
		{
			pipe(r_pipe_fd);
			child_p2 = fork();
			if (child_p2 == 0)
			{
				printf("------------------------------in child2[%s]\n", node->content->data);

				if (save_pipe_fd[P_READ])
				{
					close(save_pipe_fd[P_WRITE]);
					dup2(save_pipe_fd[P_READ], 0);
					close(save_pipe_fd[P_READ]);
				}
				close(r_pipe_fd[P_READ]);
				dup2(r_pipe_fd[P_WRITE], 1);
				close(r_pipe_fd[P_WRITE]);
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
			printf("the child2 pid=[%d]::pid_status=[%d][%d][%d][%d][%d][%d][%d][%d]\n", child_p2, WIFEXITED(pid_status), WEXITSTATUS(pid_status), WIFSIGNALED(pid_status), WTERMSIG(pid_status), WCOREDUMP(pid_status),  WIFSTOPPED(pid_status), WSTOPSIG(pid_status), WIFCONTINUED(pid_status));//del
		}
		printf("2::in parent pid=[%d]\n", getpid());

		// close(save_pipe_fd[P_READ]);
		// close(save_pipe_fd[P_WRITE]);

		// save_pipe_fd[P_READ] = dup(r_pipe_fd[P_READ]);
		// save_pipe_fd[P_WRITE] = dup(r_pipe_fd[P_WRITE]);
		// close(r_pipe_fd[P_WRITE]);
		// close(r_pipe_fd[P_READ]);

		close(save_pipe_fd[P_WRITE]);
		dup2(save_pipe_fd[P_READ], 0);
		close(save_pipe_fd[P_READ]);

		*exit_status = WEXITSTATUS(pid_status);
		printf("end all child process:exit status=[%d]\n", *exit_status);
	}
	else
	{
		printf("---in simple command--------------\n");
		exec_command(node->content, env, exit_status);
	}
	printf("---exec end------------------------\n");
}
