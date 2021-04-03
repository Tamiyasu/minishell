/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_save_pwd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 16:06:13 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 16:17:33 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

static void	save_getcwd(t_env *env)
{
	char	*cwdir;

	cwdir = getcwd(NULL, 0);
	if (cwdir == NULL)
	{
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return ;
	}
	env->unset_pwd = ft_strdup(cwdir);
	free(cwdir);
}

static void	save_oldpwd(t_env *env)
{
	int	idx;

	idx = env_search(env->data, "OLDPWD");
	if (idx == -1
		|| ft_strcmp(env->data[idx], "OLDPWD") == 0
		|| ft_strcmp(env->data[idx], "OLDPWD=") == 0)
		return ;
	env->oldpwd_data = ft_strdup(&env->data[idx][7]);
}

void		unset_save_pwd(char *token_data, t_env *env)
{
	if (ft_strcmp(token_data, "PWD") == 0)
	{
		env->pwd_flag = -1;
		if (env->pwd_data != NULL)
			free(env->pwd_data);
		env->pwd_data = ft_strdup("");
		if (env->unset_pwd != NULL)
		{
			free(env->unset_pwd);
			env->unset_pwd = NULL;
		}
		save_getcwd(env);
	}
	else if (ft_strcmp(token_data, "OLDPWD") == 0)
	{
		env->oldpwd_flag = -1;
		if (env->oldpwd_data != NULL)
		{
			free(env->oldpwd_data);
			env->oldpwd_data = NULL;
		}
		save_oldpwd(env);
	}
}
