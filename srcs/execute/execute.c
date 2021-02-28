/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/28 19:55:05 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"


// typedef struct	s_parser_node
// {
//     struct	s_parser_node *l_node;
//     struct	s_parser_node *r_node;
//     t_lsttoken *content;
// }				t_parser_node;


/*
** コマンドを実行する　returnで終了ステータスを返す.
*/
int	exec_command(t_lsttoken *token, t_env *env, int *exit_status)
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
	else if (ft_strcmp(token->data, "exit") == 0)
	{
		return (execute_exit(token, exit_status));
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
	else
	{
		*exit_status = execute_execve(token, env);
	}
	return (1);
}

int	execute(t_lsttoken *token, t_env *env, int *exit_status)
{
/*
	[env] [|] [grep] [PATH]
	 	  [|]
(l_node) /	 \ (r_node)
	[env]	  [grep][PATH]
*/

	//if (セミコロン)
		// 前回のpipe_fdをリセット（使わない）
		//node = node->l_node;
	//if (pipe)
		//パイプライン作成
		//fork()
		//子プロセスでl_nodeのcommand実行
		//親プロセスでr_nodeのcommand実行

	int		pipe_fd[2];
	pid_t	pid;
	int		pid_status;

	if (pipe(pipe_fd) == -1)
	{
		ft_putendl_fd(strerror(errno), 1);
	}
	pid = fork();
	if (pid == -1) /* forkのエラー(プロセス複製失敗) */
	{
		ft_putendl_fd(strerror(errno), 1);
		return(errno);
	}
	if (pid == 0) /* 以下のコードが子プロセスで実行される */
	{
		printf("in child pid=[%d]\n", getpid());//del
		close(pipe_fd[0]);
		dup2(pipe_fd[1], 1);
		close(pipe_fd[1]);
		*exit_status = execute_env(env->data);//exec_command();
	}
	else /* 以下のコードが親プロセスで実行される  */
	{
		printf("in parent pid=[%d]\n", getpid());//del
		if((waitpid(pid, &pid_status, 0) == -1))
		{
			// printf("in parent pid=[%d]\n", getpid());//del
			ft_putendl_fd(strerror(errno), 1);
			return (errno);
		}
		close(pipe_fd[1]);
		dup2(pipe_fd[0], 0);
		close(pipe_fd[0]);
		execute_execve(token, env);//exec_command();
	}
	printf("out parent pid=[%d]\n", getpid());//del
	return (WEXITSTATUS(pid_status));
}
