/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/04 17:19:44 by ysaito           ###   ########.fr       */
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
	printf("in exex_command\n");
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
	return (*exit_status);
}

int	execute(t_parser_node *node, t_env *env, int *exit_status)
{
	if (node->content->flag == FT_SEMICOLON_F)
	{
		printf("root[%s]\n", node->content->data);
	//	t_parser_node	*copy_node_root = node;
		//まず左
		node = node->l_node;
		printf("root->left->node[%s]\n", node->content->data);
	}
	if (node->content->flag == FT_PIPE_F)
	{
		t_parser_node	*copy_node = node;
		printf("copy_node[%s]\n", copy_node->content->data);
		copy_node = copy_node->r_node;
		printf("copy_node(pipe)->r_node[%s]\n", copy_node->content->data);
		int	pipe_fd[2];
		pipe(pipe_fd);
		int	pid_status;
		pid_t child_p1 = fork();
		if  (child_p1 == 0)
		{
			node = node->l_node;
			printf("child1[%s]\n", node->content->data);
			close(pipe_fd[0]);
			printf("before dup2\n");
			int rc = dup2(pipe_fd[1], 1);
			if (rc == -1)
			{
				printf("%s\n", strerror(errno));
			}
			printf("after dup2\n");
			close(pipe_fd[1]);
			*exit_status = exec_command(node->content, env, exit_status);
		}
		pid_t child_p2 = fork();
		if (child_p2 == 0)
		{
			copy_node = copy_node->r_node;
			printf("child2[%s]\n", copy_node->content->data);
			close(pipe_fd[1]);
			dup2(pipe_fd[0], 0);
			close(pipe_fd[0]);
			*exit_status = exec_command(copy_node->content, env, exit_status);
		}
		else
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			waitpid(child_p1, &pid_status, 0);
			waitpid(child_p2, &pid_status, 0);
			*exit_status = WEXITSTATUS(pid_status);
		}
	}
// 	node = node->l_node;
// 	if (node == NULL)
// 	{
// 		//右枝実行
// 	}
// 	//終端確認
	return (1);
}
