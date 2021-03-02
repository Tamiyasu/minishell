/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 22:57:58 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "execute.h"
#include "libft.h"
#include "get_next_line.h"
#include "parser.h"

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

t_lsttoken *find_first_commnd_node(t_parser_node *node)
{
	printf("node : %p\n", node);
	printf("content : %p\n", node->content);
	printf("flag : %d\n", node->content->flag);
	while (node->content->flag != FT_COMMAND_F)
		node = node->l_node;
	return (node->content);
}


void	msh_loop(t_env *env, int *exit_status)
{
	char	*line;
	int		loop_status;
	t_lsttoken *token;
	t_parser_node *node;

	line = NULL;
	loop_status = 1;
	token =  NULL;
	while (loop_status)
	{
		ft_putstr_fd("minishell>> ", 1);
		if (get_next_line(&line) == GNL_ERR)
		{
			return ;//error処理(free等)してexit。
		}
		token = lexer(line);
		if (token == NULL)
			return ;
		node = parser(token);
		token = find_first_commnd_node(node);

		printf("node * : %p\n", node);
		node_print(node, 0);
		//////////////* check msh_lexer */
		// t_lsttoken *copy_token = token;


		// for (int count = 0; copy_token != NULL; count++)
		// {
		// 	printf("count[%d]=[%s]\n", count, copy_token->data);
		// 	copy_token = copy_token->next;
		// }
		//free_lst(&copy_token);
		//////////////* check msh_lexer del*/

		/* parse（トークンを、コマンド・オプション・環境変数等に分ける。*/
		//args = ft_split(line, ' ');

		/* execute（解析されたコマンドを実行）*/
		loop_status = execute(token, env, exit_status); //exitコマンド実行時にreturn(0)がくる
		free_tree(&node);
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



/*
echo
cd
pwd
export
unset
env
exit
*/

/*
** リダイレクト
**		例えば echo aaa > test.txt
**	//fopen("test.txt", "w");
**	//fork();
**	//dup2();
**	//execve();
*/

/*
** パイプ
**	//pipe();	パイプと2つのディスクリプタを作成
**	//fork();
**	//dup2();
**	//使わないfpをclose();
**	//execve();
**	親プロセス側も
**	//dub2();
**	//close();
*/

/*
 環境変数の
 [配列管理]
 [リスト構造体管理]
*/
