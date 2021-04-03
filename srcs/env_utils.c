/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 13:53:18 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 15:38:02 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"

void	env_free(t_env *env)
{
	free_args(env->data);
	if (env->pwd_data != NULL)
		free(env->pwd_data);
	if (env->unset_pwd != NULL)
		free(env->unset_pwd);
	if (env->oldpwd_data != NULL)
		free(env->oldpwd_data);
}

void	env_init(t_env *env)
{
	env->num = 0;
	env->oldpwd_flag = 0;
	env->pwd_flag = 0;
	env->shlvl_flag = 0;
	env->pwd_data = NULL;
	env->unset_pwd = NULL;
	env->oldpwd_data = NULL;
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

void	env_update_pwddata(t_env *env, char *cd)
{
	char *cwdir;

	cwdir = cwd_wrapper(env, cd);
	if (cwdir == NULL)
	{
		error_str("error retrieving current directory: ");
	}
	else
	{
		if (env->oldpwd_data)
			free(env->oldpwd_data);
		if (env->pwd_data)
		{
			env->oldpwd_data = ft_strdup(env->pwd_data);
			free(env->pwd_data);
		}
		env->pwd_data = ft_strdup(cwdir);
	}
}

void	error_shlvl(int shlvl_num)
{
	char	*shlvl_str;

	shlvl_str = ft_itoa(shlvl_num + 1);
	error_str(") too high, resetting to 1");
	error_str(shlvl_str);
	error_str("warning: shell level (");
	ft_putendl_fd(error_str("minishell: "), STDERR_FILENO);
	error_str(NULL);
	free(shlvl_str);
	shlvl_str = NULL;
}
