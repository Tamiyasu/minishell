/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/28 17:37:09 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
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

void	free_lst(t_lsttoken *token)
{
	t_lsttoken *temp;

	while (token != NULL)
	{
		temp = token->next;
		free(token->data);
		free(token);
		token = temp;
	}
	token = NULL;
}

void	msh_loop(t_env *env, int *exit_status)
{
	char	*line;
	int		loop_status;
	t_lsttoken *token;

	line = NULL;
	loop_status = 1;
	token =  NULL;
	while (loop_status)
	{
		ft_putstr_fd("minishell>> ", 1);
		if (get_next_line(&line) == GNL_ERR)/* read（標準入力からコマンドを読み取る) */
		{
			return ;//error処理(free等)してexit。
		}
		token = msh_lexer(line);/* lexer (読み取った入力をトークン(意味のある単語)に分ける) */
		if (token == NULL)
			return ;

		//////////////* check msh_lexer */
		printf("------[check msh_lexer]-------------\n");
		t_lsttoken *copy_token = token;
		for (int count = 0; copy_token != NULL; count++)
		{
			printf("count[%d]=[%s]\n", count, copy_token->data);
			copy_token = copy_token->next;
		}
		printf("------------------------------\n");
		free_lst(copy_token);
		//////////////* check msh_lexer del*/

		/* parse（トークンを、コマンド・オプション・環境変数等に分ける。*/
		//args = ft_split(line, ' ');

		/* execute（解析されたコマンドを実行）*/
		//loop_status = msh_execute(token, env, exit_status); //exitコマンド実行時にreturn(0)がくる
		loop_status = execute(token, env, exit_status); //exitコマンド実行時にreturn(0)がくる
		free_lst(token);
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
