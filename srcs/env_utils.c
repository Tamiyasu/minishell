/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 13:53:18 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 22:02:16 by tmurakam         ###   ########.fr       */
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
