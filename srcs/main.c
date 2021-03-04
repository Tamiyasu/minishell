/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/04 17:21:33 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "parser.h"
#include "expansion.h"
#include "execute.h"
#include "libft.h"
#include "get_next_line.h"

#include <unistd.h>

void	free_args(char **args)
{
	int	idx;

	idx = 0;
	if (args == NULL)
	{
		return ;
	}
	while (args[idx] != NULL)
	{
		free(args[idx]);
		idx++;
	}
	free(args);
	args = NULL;
}

// void	free_lst(t_lsttoken **token)
// {
// 	t_lsttoken *temp;
// 	t_lsttoken *temp_next;

// 	temp = *token;
// 	while (temp != NULL)
// 	{
// 		temp_next = temp->next;
// 		free(temp->data);
// 		free(temp);
// 		temp = temp_next;
// 	}
// 	*token = NULL;
// }

// t_lsttoken *find_first_commnd_node(t_parser_node *node)
// {
// 	printf("node : %p\n", node);
// 	printf("content : %p\n", node->content);
// 	printf("flag : %d\n", node->content->flag);
// 	while (node->content->flag != FT_COMMAND_F)
// 		node = node->l_node;
// 	return (node->content);
// }


void	msh_loop(t_env *env, int *exit_status)
{
	char	*line;
	int		loop_status;
	t_lsttoken *token_list;
	t_parser_node *node;


	line = NULL;
	loop_status = 1;
	token_list =  NULL;
	while (loop_status)
	{
		ft_putstr_fd("minishell>> ", 1);
		if (get_next_line(&line) == GNL_ERR)/* read（標準入力からコマンドを読み取る) */
		{
			return ;//error処理(free等)してexit。
		}
		token_list = lexer(line);/* lexer (読み取った入力をトークン(意味のある単語)に分ける) */
		if (token_list == NULL)
		{
			continue ;
		}
		////////////* check msh_lexer */
		printf("------[check msh_lexer]-------------\n");
		t_lsttoken *copy_token = token_list;
		for (int count = 0; copy_token != NULL; count++)
		{
			printf("count[%d]=[%s]\n", count, copy_token->data);
			copy_token = copy_token->next;
		}
		printf("------------------------------\n");
		free_lst(&copy_token);
		//////////////* check msh_lexer del*/

		node =  parser(token_list);
		//token = find_first_commnd_node(node);
		printf("after parser node root=[%s]\n", node->content->data);


		expansion(node, exit_status);


		/* execute（解析されたコマンドを実行）*/
		loop_status = execute(/*token*/node, env, exit_status); //exitコマンド実行時にreturn(0)がくる
		free_tree(&node);
		//free_lst(token);
		free(line);
		line = NULL;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_env	env;
	int		exit_status;

	argc -= argc;//del
	argv -= (long)argv;//del
	exit_status = 0;//de;

	msh_env_init(&env);
	msh_env_make_data(&env, envp);
	if (env.data == NULL)
	{
		return (EXIT_FAILURE);
	}
	msh_loop(&env, &exit_status);
	msh_env_free(&env);
	return (exit_status);
}
