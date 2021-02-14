/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/09 16:02:21 by ysaito           ###   ########.fr       */
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
		/* read（標準入力からコマンドを読み取る) コマンドラインが複数行になる場合、EOFが来るまで読み続ける（cub3dみたいに）。 */
		if (get_next_line(&line) == GNL_ERR)
		{
			return ;//error処理(free等)してexit。
		}
		/* lexer (読み取った入力をトークン(意味のある単語)に分ける) */
		token = msh_lexer(line);
		if (token == NULL)
			return ;

		//////////////* msh_lexer機能確認 */
		printf("------[msh_lexer 機能確認]-------\n");
		t_lsttoken *copy_token = token;
		for (int count = 0; copy_token != NULL; count++)
		{
			printf("count[%d]=[%s]\n", count, copy_token->data);
			copy_token = copy_token->next;
		}
		printf("------------------------------\n");
		free_lst(copy_token);
		//////////////* msh_lexer出力確認 後で消す */

		/* parse（トークンを、コマンド・オプション・環境変数等に分ける。*/
		//args = ft_split(line, ' ');

		/* execute（解析されたコマンドを実行）*/
		loop_status = msh_execute(token, env, exit_status); //exitコマンド実行時にreturn(0)がくる
		free_lst(token);
		free(line);
		line = NULL;
	}
}

void	msh_make_envstrct(t_env *env, char **envp)
{
	int	idx;

	env->num = 0;
	while (envp[env->num] != NULL)
	{
		env->num++;
	}
	env->data = malloc(sizeof(char *) * (env->num + 1));
	if (env->data == NULL)
		return ;
	idx = 0;
	while (envp[idx] != NULL)
	{
		env->data[idx] = ft_strdup(envp[idx]);
		idx++;
	}
	env->data[idx] = NULL;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_env	env;
	int		exit_status;

	argc = 0;//del
	argv  = NULL;//del
	exit_status = 0;//de;

	msh_make_envstrct(&env, envp);
	if (env.data == NULL)
	{
		return (EXIT_FAILURE);
	}
	msh_loop(&env, &exit_status);
	free_args(env.data);
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
