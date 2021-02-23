/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:01:07 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/23 18:20:09 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

///////////////////////////////////////
///			External functs			///
///////////////////////////////////////
/*
malloc
free
write
open, read ,close
stat, lstat, fstat
opendir, readdir, closedir
strerror, errno
/////////////////////////////////////////////////////////////////////////////
//	以下は、シェルがどのような仕組みで動いているのか知ってからの方が理解しやすいかな...	//
////////////////////////////////////////////////////////////////////////////

[

]
	--	プロセスIDを表すことが可能な符号付き整数型です。GNU Cライブラリでは、これはintです。
	OSによって、プロセスID を格納する型の種類が異なる。shortを使うOSがあれば、intを使うOSもある。
	pid_tは必ずOSが使う型に設定される。

fork
	-- 呼び出し元プロセスを複製して新しいプロセスを生成する。
	-- #include <unistd.h>
		pid_t fork(void);
	-- [return]
		成功した場合、親プロセスには子プロセスの PID が返され、子プロセスには 0 が返される。
		失敗した場合、親プロセスに -1 が返され、子プロセスは生成されず、errno が適切に設定される。
wait
	-- プロセスの終了を待つ。
waitpid
	-- pid 引き数で指定した子プロセスの状態変化が起こるまで、呼び出し元のプロセスの実行を一時停止する。
	   デフォルトでは、 waitpid() は子プロセスの終了だけを待つが、この動作は options 引き数により変更可能である。
wait3, wait4
	-- BSD スタイルでプロセスの状態変化を待つ。
	--  #include <sys/wait.h>
		pid_t wait(int *stat_loc);
		pid_t wait3(int *stat_loc, int options, struct rusage *rusage);
		pid_t wait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage);
		pid_t waitpid(pid_t pid, int *stat_loc, int options);
	-- [return]
			wait(): 成功すると、終了した子プロセスのプロセスID を返す。 エラーの場合 -1 を返す。
			waitpid(): 成功すると、状態が変化した子プロセスのプロセスID を返す。 WNOHANG が指定されていて、 pid で指示された子プロセスが一つ以上存在するが、どの子プロセスでも 状態変化が起こっていなかった場合は、 0 を返す。 エラーの場合 -1 を返す。
			エラーの場合、これらのシステムコールはいずれも errno に適切な値を設定する。

signal(3)
	-- シグナルを使用すると、ドメインの外部からプロセスを操作できるだけでなく、
	   プロセスがそれ自体またはそれ自体のコピー（子）を操作できるようになります。
	--  #include <signal.h>
		typedef void (*sighandler_t)(int);
		sighandler_t signal(int signum, sighandler_t sighandler);
	-- [return]
			呼び出しが成功すると、前のアクションが返されます。
			それ以外の場合は、SIG_ERRが返され、グローバル変数errnoが設定されます。
kill(2)
	-- プロセスにシグナルを送る。
	--  #include <sys/types.h>
		#include <signal.h>
		int kill(pid_t pid, int sig);
	-- [return]
			成功した場合 (少なくとも一つのシグナルが送信された場合)、 0 が返される。
			エラーの場合 -1 が返され、 errno が適切に設定される。
exit(2)
getcwd(3)
	--	呼び出したプロセスのカレントワーキングディレクトリ名(絶対パス名)を取得。
		カレントワーキングディレクトリの絶対パス名を buf で示された size 長の配列にコピーする。
		終端のヌルバイトも含めた、絶対パス名の長さが size バイトを超えている場合は、返り値として NULL が返り errno に ERANGE がセットされる。
		アプリケーションはこのエラーをチェックし、 必要に応じてより長いバッファーを用意すべきである。
	--  #inculude <unistd.h>
		char *getcwd(char *buf, size_t size);
	-- [return]
			成功すると、これらの関数はカレントワーキングディレクトリの絶対パス名 が入った文字列へのポインターを返す。
			失敗した場合、これらの関数は NULL を返し、 errno にエラーを示す値を設定する。 buf が指す配列の内容は未定義である。
chdir(2)
	--	path に指定されたディレクトリに変更する。
	--	#include <unistd.h>
		int chdir(const char *path);
	-- 成功した場合は 0 が返される。エラーの場合は -1 が返され、 errno が適切に設定される。

execve(2)
	--	pathに指定されたファイルを実行する(呼び出し元のプロセスを新しいプロセスに変換します。)
	--	 #include <unistd.h>
		int	execve(const char *path, char *const argv[], char *const envp[]);
			const *argv[] == 新しいプロセスで使用できるようにする引数リストを構成します。配列には少なくとも1つの引数が存在する必要があります。カスタムでは、最初の要素は実行されたプログラムの名前である必要があります（たとえば、パスの最後のコンポーネント）。
			const *envp[] == この配列へのポインタは通常、グローバル変数environに格納されます。これらの文字列は、コマンドへの直接の引数ではない情報を新しいプロセスに渡します。
	--	execve（）呼び出しの結果としてプログラムが実行されると、次のように入力されます。

			main（argc、argv、envp）//int argc, char ** argv、** envp;

		ここで、argcはargv内の要素の数（「argcount」）であり、argvは引数自体への文字ポインターの配列を指します。
	--	[return]
			成功すると execve() は返らない。(現在のプロセスイメージを新しいプロセスイメージでオーバーレイするため、成功した呼び出しには戻るプロセスがありません。)
			エラーの場合は -1 を返し、 errno を適切に設定する。
dup(2)
	--	既存のファイル記述子を複製する.
	--	#include <unistd.h>
		int	dup(int fildes);
	--	ファイルディスクリプター fildes のコピーを作成し、 最も小さい番号の未使用のディスクリプターを 新しいディスクリプターとして使用する。
		成功が返された場合には、 古いファイルディスクリプターと新しいファイルディスクリプターは 互いに可換なものとして使うことができる。 2つのファイルディスクリプターは同じファイル記述 (description) (open(2) 参照) を参照しており、したがってファイルオフセットやファイル状態フラグが 共有される。
	--	[return]
			成功すると、新しいディスクリプターを返す。
			エラーの場合、-1 を返し、 errno を適切に設定する。
dup2(2)
	--	int	dup2(int fildes, int fildes2);
	--	fildes2のfd番号のデータストリームをfildesに指定されたfd番号のデータストリームに複製する。
	dup() と同じ処理を実行するが、 番号が最も小さい未使用のファイルディスクリプターを使用する代わりに、 fildes2 で指定されたディスクリプター番号を使用する。 ディスクリプター newfd が以前にオープンされていた場合には、 黙ってそのディスクリプターをクローズしてから再利用する。
pipe(2)
	--	#include <unistd.h>
		int	pipe(int fildes[2]);
	--	プロセス間通信用の記述子ペアを作成する。
			パイプを生成する。 パイプは、プロセス間通信に使用できる単方向のデータチャネルである。
			配列 fildes は、パイプの両端を参照する二つのファイルディスクリプターを 返すのに使用される。
			fildes[0] がパイプの読み出し側、 fildes[1] がパイプの書き込み側である。 パイプの書き込み側に書き込まれたデータは、 パイプの読み出し側から読み出されるまでカーネルでバッファーリングされる。
	--	[return]
			成功した場合は 0 が返される。エラーの場合は -1 が返され、 errno が適切に設定される。
*/

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>//strerror
# include <errno.h>//errno
# include <sys/wait.h>

typedef struct		s_lsttoken
{
	char				*data;
	int			flag;
	struct s_lsttoken	*next;
}					t_lsttoken;

/*
** msh_lexer
*/
enum flags
{
	CHAR_PIPE = '|',
	CHAR_SEMICOLON = ';',
	CHAR_LESSSIGN = '<',
	CHAR_GREATERSIGN = '>',
	//CHAR_DOLLAR = '$',
	CHAR_SQUOTE = '\'',
	CHAR_DQUOTE = '\"',
	CHAR_ESCAPESEQ = '\\',
	CHAR_SPACE = ' ',
	CHAR_TAB = '\t',
	CHAR_NEWLINE = '\n',
	//CHAR_QUESTION_MARK = '?'
	CHAR_NULL = 0,
};

enum
{
	STATE_GENERAL,
	STATE_IN_SQUOTE,
	STATE_IN_DQUOTE,
	STATE_IN_ESCAPESEQ,
};

typedef struct	s_env
{
	char		**data;
	int			num;
	char		*pwd_data; //PWD存在しようがしないが、cdされるごとに常にcwdirに更新される。unset PWDされた際は、空文字が入る
	char		*unset_pwd; //unset PWD が実行されたcwdirを保存する
	int			oldpwd_flag; //存在しない時は-1となる.
	int			pwd_flag; //存在しない時は-1となる.
}				t_env;

void		free_args(char **args);
void		free_lst(t_lsttoken *token);
t_lsttoken	*msh_lexer(char *input);

/*
** msh_env.c
*/
void	msh_env_init(t_env *env);
void	msh_env_make_data(t_env *env, char **envp);
int		msh_env_search(char **env_data, char *variable_name);
void	msh_env_update_pwddata(t_env *env);
void	msh_env_free(t_env *env);

#endif