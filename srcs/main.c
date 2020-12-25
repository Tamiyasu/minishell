/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2020/12/25 18:13:18 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "get_next_line.h"

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

// int	msh_num_builtins(char **builtin_str)
// {
// 	return (sizeof(builtin_str) / sizeof(char *));
// }

/*
** 子プロセスの作成と新しいプロセスの実行
*/
int		msh_launch(char **args, char **envp)
{
	pid_t	pid;
	// pid_t	wait_pid;
	char	*command_path;
	// int		status;

	envp = NULL;//del;

	pid = fork();//shell自身の複製（子プロセス）を作成
	if (pid == -1)
	{
		strerror(errno);
		//error処理で終了？->fork失敗ってことは、複製ができていないからこのままexitしたら、minishell自体が終了してしまうと思う。
	}
	else if (pid == 0)//子プロセスだぞって返ってきたら
	{
		command_path = ft_strjoin("/bin/", args[0]);
		if (execve(command_path/*args[0]*/, args/*コマンドライン引数、オプション*/, envp) == -1)//第一引数がpathなら、args[0]にある実行したいコマンドで、振り分け必要？
		// if (execvp(args[0], args) == -1)
		{
			strerror(errno);//perror("msh_launch");//execve()のerror処理 strerror(errno);//もし実装していないコマンドが入力されたら、ここで適当なエラー分出力するべきでは。
			// return (1);
		}
		// exit(EXIT_FAILURE);//leak check!!! //ここでexit使っているのはなんで？要確認
	}
	// else//親はコマンドの実行が終了するのを待つ。
	// {
	// 	while (1)
	// 	{
	// 		wait_pid = waitpid(pid, &status, WUNTRACED);//WUNTRACED:子プロセスが停止した場合にも復帰する。
	// 		if (WIFEXITED(status) && WIFSIGNALED(status))
	// 		{
	// 			break;
	// 		}
	// 		printf("msh_launch\n");//del
	// 	}
	// }
	return (1);
}

int		msh_execute(char **args, char **envp)
{
	// /* 関数ポインタの配列 */
	// int	(*builtin_func[])(char **) = {
	// 	msh_cd,
	// 	msh_exit,
	// 	msh_pwd
	// };
	// /* 実装コマンドの文字列配列 */
	// char	*builtin_str[] = {
	// 	"cd",
	// 	"echo",
	// 	"exit",
	// 	"pwd"
	// };
	int	args_len;
	int	idx;

	if (args[0] == NULL)//コマンドが何も入力されなかった時
	{
		return (1);
	}
	args_len = ft_strlen(args[0]);
	idx = 0;

	///////////////
	// for (int i = 0; args[i] != NULL; i++)
	// {
	// 	printf("args[%d]=[%s]\n", i, args[i]);
	// }
	//////////////
	while (idx < 5)
	{
		if (ft_strncmp(args[0], "cd", (args_len + 1)) == 0)
		{
			return (msh_cd(args));
		}
		if (ft_strncmp(args[0], "env", (args_len + 1)) == 0)
		{
			return (msh_env(envp));
		}
	 	if (ft_strncmp(args[0], "exit", (args_len + 1)) == 0)//入力されたコマンドと、コマンド名を格納した配列を比べて、どのindexに格納されているか探す。
		{
			//return (0);でもいい？
			return (msh_exit(args));//見つけたindexに格納されている関数を実行。
		}
		if (ft_strncmp(args[0], "pwd", (args_len + 1)) == 0)
		{
			return (msh_pwd(args));
		}
		idx++;
	}
	// while (idx < msh_num_builtins(builtin_str))
	// {
	// 	if (ft_strncmp(args[0], builtin_str[idx], (args_len + 1)) == 0)
	// 	{
	// 		return (*builtin_func[idx])(args);
	// 	}
	// 	idx++;
	// }
	return (msh_launch(args, envp));//未実装のコマンドはexecveを使ってコマンドを実行。
}

void	minish_loop(char *envp[])
{
	char	*line;
	char	**args;
	// char	**envp;//execve()の第３引数(Environment variable pointer)
	int		status;

	line = NULL;
	// envp = NULL;
	status = 1;
	while (status)
	{
		ft_putstr_fd("minishell>> ", 1);
	/*
	** read（標準入力からコマンドを読み取る)
	** 		コマンドラインが複数行になる場合、EOFが来るまで読み続ける（cub3dみたいに）。
	*/
		if (get_next_line(&line) == GNL_ERR)
		{
			//error処理(free等)してexit。
			return ;//error処理してexit書くならいらない。
		}
	/*
	** parse（コマンド文字列をプログラムと引数に分割）
	** 		とりあえず" "(space)で。
	** 		コマンドライン引数を分割して、コマンド・オプション・環境変数等に分ける。
	*/
		args = ft_split(line, ' ');
	/*
	** execute（解析されたコマンドを実行）
	**		fork,execve,waitpid
	*/
		status = msh_execute(args, envp);
		free(line);
		line = NULL;
		free_args(args);
		// printf("minish_loop\n");//del
	}
	//status != 1　になる == 何かしらのエラー
	//error出力
	//終了
}

int	main(int argc, char *argv[], char *envp[])
{
	printf("argc=[%d] argv[0]=[%s]\n", argc, argv[0]);
	/////////////////
	//	shellの流れ	//
	/////////////////
	//1.初期化
	//2.解釈
	minish_loop(envp);
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
