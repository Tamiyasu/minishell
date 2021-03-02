/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_execve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 14:44:34 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 23:21:58 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "lexer.h"
#include "libft.h"

static int	execve_count_lst(t_lsttoken *token)
{
	int	num;

	num = 0;
	while (token != NULL)
	{
		num++;
		token = token->next;
	}
	return (num);
}

/*
** コマンド部分のtoken->dataが、絶対パスか相対パスか判断する
** [return]先頭が'.'または、'/'が含まれる時0を返す。なし→1
*/
static int	execve_check_command(char *command)
{
	int	idx;

	idx = 0;
	while (command[idx] != '\0')
	{
		if (command[0] == '.')
		{
			return (0);
		}
		if (command[idx] == '/')
		{
			return (0);
		}
		idx++;
	}
	return (1);
}

/*
** 環境変数PATHを見つけて、PATHの値を返す。
** [return]PATHの格納されたpathの配列。環境変数PATHが見つからなかったらNULL。
*/
static char	**execve_keep_envpath(t_env *env, int *relative_path)
{
	char	**env_path;
	int	idx;

	idx = msh_env_search(env->data, "PATH");
	if (idx == -1)
	{
		*relative_path = 1;
		return(NULL);
	}
	env_path = ft_split(&env->data[idx][5], ':');
	return (env_path);
}

/*
** execve()の第一引数commandを作成する。
** [return]char *command。
*/
static char *execve_format_command(t_lsttoken *token, char **env_path, int path_idx)
{
	char	*command;
	char	*tmp;

	if(token->flag == 1 || env_path == NULL)
	{
		command = ft_strdup(token->data);
		return (command);
	}
	tmp = ft_strjoin(env_path[path_idx], "/");
	command = ft_strjoin(tmp,  token->data);
	free(tmp);
	return (command);
}

/*
** execve()の第二引数argsを作成する。
** [return]char **args コマンドの引数を格納した配列。
*/
static char	**execve_format_args(t_lsttoken *token, char *command)
{
	char	**args;
	int		lst_num;
	int		idx;

	lst_num = execve_count_lst(token);
	args = malloc(sizeof(char *) * (lst_num + 1));
	if (args == NULL)
	{
		return (NULL);
	}
	idx = 0;
	args[idx] = ft_strdup(command);
	idx++;
	token = token->next;
	while (token != NULL)
	{
		args[idx] = ft_strdup(token->data);
		token = token->next;
		idx++;
	}
	args[idx] = NULL;
	return (args);
}

/*
** forkでプロセス作成、execveでコマンド実行する。
** fork(), waitpid()のエラーはここで出力。
*/
static int	execve_execute_command(char *command, char **args, t_env *env)
{
	pid_t	pid;
	int		pid_status;

	pid = fork();
	if (pid == -1)
	{
		ft_putendl_fd(strerror(errno), 1);
		return(errno);
	}
	if (pid == 0) /* 以下のコードが子プロセスで実行される */
	{
		printf("in child pid=[%d]\n", getpid());//del
		// printf("子プロセスの中身 (command : %s)\n", command);
		/*
		int i = 0;
		while(*(args + i))
		{
			printf("*(args + %d) : %s\n", i, *(args + i));
			i++;
		}
		i = 0;
		while(*((env->data) + i))
		{
			printf("*((env->data)  + %d) : %s\n", i, *((env->data) + i));
			i++;
		}
		*/

		int __r = execve(command, args, env->data);
		printf("__r : %d\n", __r);

		if (__r == -1)//コマンドの実行エラー
		{
			printf("失敗時 : errno : %d\n", errno);
			ft_putendl_fd(strerror(errno), 1);
			exit(errno);
		}
	}
	else /* 以下のコードが親プロセスで実行される  */
	{
		printf("in parent childpid=[%d]\n", pid);//del
		int __k = waitpid(pid, &pid_status, 0);
		printf("in parent pid_status=[%d]\n", pid_status);//del
		printf("in parent __k=[%d]\n", __k);//del
		/*if((__k == -1))
		{
			ft_putendl_fd(strerror(errno), 1);
			return (errno);
		}*/
		printf("in parent pid_status=[%d]\n", pid_status);//del
		//printf("the child pid=[%d]::pid_status=[%d][%d][%d][%d][%d][%d][%d][%d]\n", pid, WIFEXITED(pid_status), WEXITSTATUS(pid_status), WIFSIGNALED(pid_status), WTERMSIG(pid_status), WCOREDUMP(pid_status),  WIFSTOPPED(pid_status), WSTOPSIG(pid_status), WIFCONTINUED(pid_status));//del
	}
	printf("WIFEXITED(pid_status) : %d\n", WIFEXITED(pid_status));
	printf("WEXITSTATUS(pid_status) : %d\n", WEXITSTATUS(pid_status));
	return (WEXITSTATUS(pid_status));
}

int	execve_output_error(t_lsttoken *token, char *error_str, int exit_status)
{
	ft_putstr_fd("minishell: ", 1);
	ft_putstr_fd(token->data, 1);
	ft_putstr_fd(": ", 1);
	ft_putendl_fd(error_str, 1);
	print_token(token, "here 6 ******** ");
	return (exit_status);
}

int			execute_execve(t_lsttoken *token, t_env *env)
{
	char	*command;
	char	**args;
	char	**env_path;
	int		relative_path;
	int		path_idx;
	int		execve_rc;

	env_path = NULL;
	path_idx = 0;
	relative_path = execve_check_command(token->data);
	if (relative_path == 1)
	{
		env_path = execve_keep_envpath(env, &relative_path);
	}
	while (1)
	{
		print_token(token, "here");
		command = execve_format_command(token, env_path, path_idx);
		args = execve_format_args(token, command);
		execve_rc = execve_execute_command(command, args, env);
		print_token(token, "here 0 ******** ");
		free(command);
		print_token(token, "here 1 ******** ");
		free_args(args);
		if (execve_rc == 0) /*work fine*/
		{
			print_token(token, "here 2 ******** ");
			free_args(env_path);
			return (0);
		}
		if (execve_rc != 2) /*error but return code is not 2*/
		{
			print_token(token, "here 3 ******** ");
			printf("execve_rc : %d\n", execve_rc);
			free_args(env_path);
			print_token(token, "here 3' ******** ");
			//return (execve_output_error(token, strerror(execve_rc), execve_rc));
			return (execve_rc);
		} 
		/* No such file or directory */
		if (relative_path == 0)
		{
			print_token(token, "here 4 ******** ");
			free_args(env_path);
			return (execve_output_error(token, strerror(execve_rc), 127));
		}
		path_idx++;
		if (env_path[path_idx] == NULL)
		{
			print_token(token, "here 5 ******** ");
			free_args(env_path);
			return (execve_output_error(token, "command not found", 127));
		}
	}
}
