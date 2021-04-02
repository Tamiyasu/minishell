/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:41:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/02 15:45:54 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "lexer.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void	cd_update_envpwd(t_env *env, char *arg_str)
{
	int	old_idx;
	int	idx;

	idx = env_search(env->data, "PWD");
	if (env->oldpwd_flag != -1)
	{
		old_idx = env_search(env->data, "OLDPWD");
		if (old_idx == -1)
		{
			old_idx = env->num++;
			env->data[env->num] = NULL;
		}
        else
		    free(env->data[old_idx]);
		if (env->pwd_flag != -1)
			env->data[old_idx] = ft_strjoin("OLDPWD=", &env->data[idx][4]);
		else
			env->data[old_idx] = ft_strjoin("OLDPWD=", env->pwd_data);
	}
	env_update_pwddata(env, arg_str);
	if (ft_strlen(error_str("")) > 0)
		ft_putendl_fd(error_str("cd: "), 2);
	error_str(NULL);
	if (env->pwd_flag != -1)
	{
		if (env->pwd_data && idx >= 0)
		{
			free(env->data[idx]);
			env->data[idx] = ft_strjoin("PWD=", env->pwd_data);
		}
	}
}

int		cd_home(t_env *env)
{
	char	*env_home;
	int		idx;

	idx = env_search(env->data, "HOME");
	if (idx == -1)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	env_home = ft_strdup(&env->data[idx][5]);
	if (ft_strcmp(env_home, "") == 0)
	{
		free(env_home);
		return (EXIT_SUCCESS);
	}
	if (chdir(env_home) == -1)
	{
		output_error("cd", strerror(ENOENT));
		free(env_home);
		return (EXIT_FAILURE);
	}
	free(env_home);
	cd_update_envpwd(env, NULL);
	return (EXIT_SUCCESS);
}

int		command_cd(t_token *token, t_env *env)
{
	char		*err_str;

	token = token->next;
	if (token == NULL)
		return (cd_home(env));
	else if (chdir(token->data) == -1)
	{
		err_str = ft_strjoin("cd: ", token->data);
		output_error(err_str, strerror(errno));
		free(err_str);
		return (EXIT_FAILURE);
	}
	cd_update_envpwd(env, token->data);
	return (EXIT_SUCCESS);
}
