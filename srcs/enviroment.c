/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 13:22:55 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 13:10:51 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	env_free(t_env *env)
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

void	env_init(t_env *env)
{
	env->num = 0;
	env->oldpwd_flag = 0;
	env->pwd_flag = 0;
	env->shlvl_flag = 0;
	env->pwd_data = NULL;
	env->unset_pwd=  NULL;
}

void	env_update_pwddata(t_env *env)
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

int		env_search(char **env_data, char *variable_name)
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

int	env_check_data(t_env *env, char **envp)
{
	int	env_num;

	env_num = 0;
	while (envp[env_num] != NULL)
		env_num++;
	env->oldpwd_flag =  env_search(envp, "OLDPWD");
	if  (env->oldpwd_flag == -1)
		env_num++;
	env->pwd_flag = env_search(envp, "PWD");
	if (env->pwd_flag == -1)
		env_num++;
	env->shlvl_flag = env_search(envp, "SHLVL");
	if (env->shlvl_flag == -1)
		env_num++;
	return (env_num);
}

void	env_make_data(t_env *env, char **envp)
{
	int	idx;

	env->num = env_check_data(env, envp);
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
	env_update_pwddata(env);
	if (env->pwd_flag == -1)
		env->data[idx++] = ft_strjoin("PWD=", env->pwd_data);
	else
	{
		free(env->data[env->pwd_flag]);
		env->data[env->pwd_flag] = ft_strjoin("PWD=", env->pwd_data);
	}
	if (env->shlvl_flag == -1)
	{
		env->data[idx++] = ft_strjoin("SHLVL=1", env->pwd_data);
	}
	else
	{
		int shlvl_num = ft_atoi(&env->data[env->shlvl_flag][6]);
		//printf("SHLVL_NUM=[%d]\n", shlvl_num);
		//numチェック
		if(shlvl_num < 0)
			shlvl_num = 0;
		else
			shlvl_num += 1;
		free(env->data[env->shlvl_flag]);
		env->data[env->shlvl_flag] = ft_strjoin("SHLVL=", ft_itoa(shlvl_num));
	}

	env->oldpwd_flag = 1;
	env->pwd_flag = 1;
	env->data[idx] = NULL;
}
