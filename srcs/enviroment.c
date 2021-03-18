/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 13:22:55 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/18 15:38:03 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	msh_env_free(t_env *env)
{
	free_args(env->data);
	if (env->pwd_data != NULL)
	{
		free(env->pwd_data);
	}
	if (env->unset_pwd != NULL)
	{
		free(env->unset_pwd);
	}
}

void	msh_env_init(t_env *env)
{
	env->num = 0;
	env->oldpwd_flag = 0;
	env->pwd_flag = 0;
	env->pwd_data = NULL;
	env->unset_pwd=  NULL;
}

/*
** env->dataを現在のカレントディレクトリに更新する
*/
void	msh_env_update_pwddata(t_env *env)
{
	char *cwdir;

	if (env->pwd_data != NULL)
	{
		free(env->pwd_data);
		env->pwd_data = NULL;
	}
	cwdir = getcwd(NULL, 0);
	if (cwdir == NULL)
	{
		strerror(errno);
		return ;
	}
	env->pwd_data = ft_strdup(cwdir);
	free(cwdir);
}

/*
** 環境変数一覧(env_data)からvariable_nameに一致した環境変数が格納されているindexを探す
** [return]variable_nameが格納されているindex。見つからなかったら-1
*/
int		msh_env_search(char **env_data, char *variable_name)
{
	int	varlen;
	int	idx;

	varlen = ft_strlen(variable_name);
	idx = 0;
	while (env_data[idx] != NULL)
	{
		if (ft_strncmp(env_data[idx], variable_name, varlen) == 0 &&
			(env_data[idx][varlen] == '=' || env_data[idx][varlen] == '\0'))
		{
			return (idx);
		}
		idx++;
	}
	return (-1);
}

/*
** 環境変数(envp)の数を数える かつ OLDPWD と PWDの有無チェック
** OLDPWD/PWDなし
** 	-> env->oldpwd_flag/env->pwd_flagに-1を入れ、環境変数num++;//後で追加するため
*/
int	msh_env_check_data(t_env *env, char **envp)
{
	int	env_num;

	env_num = 0;
	while (envp[env_num] != NULL)
	{
		env_num++;
	}
	env->oldpwd_flag =  msh_env_search(envp, "OLDPWD");
	if  (env->oldpwd_flag == -1)
	{
		env_num++;
	}
	env->pwd_flag = msh_env_search(envp, "PWD");
	if (env->pwd_flag == -1)
	{
		env_num++;
	}
	return (env_num);
}

/*
** msh_loop実行前にt_env envを作成する
*/
void	msh_env_make_data(t_env *env, char **envp)
{
	int	idx;

	env->num = msh_env_check_data(env, envp);
	env->data = malloc(sizeof(char *) * (env->num + 1));
	if (env->data == NULL)
		return ;
	idx = 0;
	while (envp[idx] != NULL)
	{
		env->data[idx] = ft_strdup(envp[idx]);
		idx++;
	}
	if (env->oldpwd_flag == -1)
		env->data[idx++] = ft_strdup("OLDPWD");
	msh_env_update_pwddata(env);
	if (env->pwd_flag == -1)
		env->data[idx++] = ft_strjoin("PWD=", env->pwd_data);
	else
	{
		free(env->data[env->pwd_flag]);
		env->data[env->pwd_flag] = ft_strjoin("PWD=", env->pwd_data);
	}
	env->oldpwd_flag = 1;
	env->pwd_flag = 1;
	env->data[idx] = NULL;
}
