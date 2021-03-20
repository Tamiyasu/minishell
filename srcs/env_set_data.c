/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 13:22:55 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 14:00:54 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		env_check_data(t_env *env, char **envp)
{
	int	env_num;

	env_num = 0;
	while (envp[env_num] != NULL)
		env_num++;
	env->oldpwd_flag = env_search(envp, "OLDPWD");
	if (env->oldpwd_flag == -1)
		env_num++;
	env->pwd_flag = env_search(envp, "PWD");
	if (env->pwd_flag == -1)
		env_num++;
	env->shlvl_flag = env_search(envp, "SHLVL");
	if (env->shlvl_flag == -1)
		env_num++;
	return (env_num);
}

int		env_set_shlvl(t_env *env, int idx)
{
	int shlvl_num;

	if (env->shlvl_flag == -1)
		env->data[idx++] = ft_strdup("SHLVL=1");
	else
	{
		shlvl_num = ft_atoi(&env->data[env->shlvl_flag][6]);
		if (shlvl_num < 0)
			shlvl_num = 0;
		else
		{
			if (shlvl_num == INT_MAX)
				shlvl_num = 0;
			else
				shlvl_num += 1;
		}
		free(env->data[env->shlvl_flag]);
		env->data[env->shlvl_flag] = ft_strjoin("SHLVL=", ft_itoa(shlvl_num));
	}
	return (idx);
}

int		env_set_pwd(t_env *env, int idx)
{
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
	return (idx);
}

void	env_set_data(t_env *env, char **envp)
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
	idx = env_set_pwd(env, idx);
	idx = env_set_shlvl(env, idx);
	env->oldpwd_flag = 1;
	env->pwd_flag = 1;
	env->data[idx] = NULL;
}
