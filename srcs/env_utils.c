/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 13:53:18 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 13:55:36 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_free(t_env *env)
{
	free_args(env->data);
	if (env->pwd_data != NULL)
		free(env->pwd_data);
	if (env->unset_pwd != NULL)
		free(env->unset_pwd);
}

void	env_init(t_env *env)
{
	env->num = 0;
	env->oldpwd_flag = 0;
	env->pwd_flag = 0;
	env->shlvl_flag = 0;
	env->pwd_data = NULL;
	env->unset_pwd = NULL;
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
