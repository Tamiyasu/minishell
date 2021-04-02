/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set_data.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 13:22:55 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/02 14:59:14 by ysaito           ###   ########.fr       */
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
	if (env->oldpwd_flag != -1)
		env_num--;
	env->pwd_flag = env_search(envp, "PWD");
	if (env->pwd_flag == -1)
		env_num++;
	else
	{
		if (env->oldpwd_flag != -1 && (env->pwd_flag > env->oldpwd_flag))
			env->pwd_flag--;
	}
	env->shlvl_flag = env_search(envp, "SHLVL");
	if (env->shlvl_flag == -1)
		env_num++;
	return (env_num);
}

int		set_shlvl_num(int shlvl_num)
{
	if (shlvl_num < 0 || shlvl_num == INT_MAX)
		shlvl_num = 0;
	else if (shlvl_num > SHLVL_MAX)
	{
		error_shlvl(shlvl_num);
		shlvl_num = 1;
	}
	else
		shlvl_num += 1;
	return (shlvl_num);
}

void	env_set_shlvl(t_env *env, int *data_idx)
{
	int		shlvl_num;
	char	*shlvl_str;

	if (env->shlvl_flag == -1)
	{
		env->data[*data_idx] = ft_strdup("SHLVL=1");
		*data_idx = *data_idx + 1;
		return ;
	}
	shlvl_num = ft_atoi(&env->data[env->shlvl_flag][6]);
	free(env->data[env->shlvl_flag]);
	if (shlvl_num == SHLVL_MAX)
	{
		env->data[env->shlvl_flag] = ft_strjoin("SHLVL=", "");
		return ;
	}
	shlvl_num = set_shlvl_num(shlvl_num);
	shlvl_str = ft_itoa(shlvl_num);
	env->data[env->shlvl_flag] = ft_strjoin("SHLVL=", shlvl_str);
	free(shlvl_str);
}

int		env_set_pwd(t_env *env, int idx)
{
	char *tmp_cwd;

	env_update_pwddata(env, NULL);
	if (env->pwd_flag == -1 && env->pwd_data)
		env->data[idx++] = ft_strjoin("PWD=", env->pwd_data);
	else
	{
		tmp_cwd = env->data[env->pwd_flag];
		if (env->pwd_data)
		{
			env->data[env->pwd_flag] = ft_strjoin("PWD=", env->pwd_data);
			free(tmp_cwd);
		}
	}
	return (idx);
}

void	env_set_data(t_env *env, char **envp)
{
	int	data_idx;
	int	idx;

	env->num = env_check_data(env, envp);
	env->data = malloc(sizeof(char *) * (env->num + 1));
	if (env->data == NULL)
		ft_enomem();
	data_idx = 0;
	idx = 0;
	while (envp[idx] != NULL)
	{
		if (idx != env->oldpwd_flag)
			env->data[data_idx++] = ft_strdup(envp[idx]);
		idx++;
	}
	data_idx = env_set_pwd(env, data_idx);
	if (0 < ft_strlen(error_str("")))
		ft_putendl_fd(error_str("shell-init: "), STDERR_FILENO);
	error_str(NULL);
	env_set_shlvl(env, &data_idx);
	env->oldpwd_flag = 1;
	env->pwd_flag = 1;
	env->data[data_idx] = NULL;
}
