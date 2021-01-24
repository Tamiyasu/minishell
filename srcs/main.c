/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/01/24 19:18:00 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "get_next_line.h"

#include <unistd.h>

void	lst_free(t_lsttoken *token)
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

void	minish_loop(/*char *envp[]*/)
{
	char	*line;
	// char	**args;
	int		status;
	t_lsttoken *token;

	line = NULL;
	status = 1;
	token =  NULL;
	while (status)
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
		/////////////
		for (int count = 0; token != NULL; count++)
		{
			printf("count[%d]=[%s]\n", count, token->data);
			token = token->next;
		}
		/////////////

		/* parse（トークンを、コマンド・オプション・環境変数等に分ける。*/
		//args = ft_split(line, ' ');
		/* execute（解析されたコマンドを実行）*/
		//status = msh_execute(args, envp);
		// // free(line);
		// // line = NULL;
		// free_args(args);
		lst_free(token);
		free(line);
		line = NULL;
		// status = 0;//del
	}
	//status != 1　になる == 何かしらのエラー
	//error出力
	//終了
}

int	main(/*int argc, char *argv[], char *envp[]*/)
{
	//printf("argc=[%d] argv[0]=[%s]\n", argc, argv[0]);
	////////////
	// for (int x = 0; envp[x] != NULL; x++)
	// {
	// 	printf("%s\n", envp[x]);
	// }
	/*
	** envpを配列からリスト構造に移す。
	*/
	////////////
	/////////////////
	//	shellの流れ	//
	/////////////////
	//1.初期化
	//2.解釈
	minish_loop(/*envp*/);
	////(1)read（標準入力からコマンドを読み取る）
	////(2)parse（コマンド文字列をプログラムと引数に分割）
	////(3)execute（解析されたコマンドを実行）
	//3.終了
	return (EXIT_SUCCESS);
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
