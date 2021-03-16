/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:41:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/16 23:13:44 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "lexer.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

static void	cd_update_envpwd(t_env *env)
{
	int	old_idx;
	int	idx;

	idx = msh_env_search(env->data, "PWD");
	if (env->oldpwd_flag != -1) /* OLDPWDの更新*/
	{
		old_idx = msh_env_search(env->data, "OLDPWD");
		free(env->data[old_idx]);
		if (env->pwd_flag != -1)
		{
			env->data[old_idx] = ft_strjoin("OLDPWD=", &env->data[idx][4]);
		}
		else
		{
			env->data[old_idx] = ft_strjoin("OLDPWD=", env->pwd_data);
		}
	}
	msh_env_update_pwddata(env); /*env->pwd_dataの更新*/
	if  (env->pwd_flag != -1)
	{
		free(env->data[idx]);
		env->data[idx] = ft_strjoin("PWD=", env->pwd_data);
	}
}

int	cd_home(t_env *env)
{
	char	*env_home;
	int		idx;

	idx = msh_env_search(env->data, "HOME");
	if (idx == -1)
	{
		ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	env_home = ft_strdup(&env->data[idx][5]);
	if (chdir(env_home) == -1)
	{
		output_error("cd", strerror(ENOENT));
		free(env_home);
		return (EXIT_FAILURE);
	}
	free(env_home);
	cd_update_envpwd(env);
	return (EXIT_SUCCESS);
}

int	execute_cd(t_lsttoken *token, t_env *env)
{
	char *err_str;
	struct stat stat_buf;
	int err_cord;

	token = token->next;
	if (token == NULL)
	{
		return (cd_home(env));
	}
	else if (chdir(token->data) == -1)
	{
		err_cord = ENOENT;
		if(!stat(token->data, &stat_buf) && (stat_buf.st_mode & S_IFMT) != S_IFDIR)
			err_cord = ENOTDIR;
		err_str = ft_strjoin("cd: ", token->data);
		output_error(err_str, strerror(err_cord));
		free(err_str);
		return (EXIT_FAILURE);
	}
	cd_update_envpwd(env);
	return (EXIT_SUCCESS);
}
