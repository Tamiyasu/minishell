/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:41:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/04 20:51:14 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "lexer.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void	cd_update_envpwd(t_env *env, char *aim_dir)
{
	int	old_idx;
	int	idx;

	idx = env_search(env->data, "PWD");
	if (env->oldpwd_flag != -1)
	{
		old_idx = env_search(env->data, "OLDPWD");
		if (old_idx == -1)
			env->data[env->num + 1] = NULL;
		if (old_idx == -1)
			old_idx = env->num++;
		else
			free(env->data[old_idx]);
		if (env->pwd_flag != -1)
			env->data[old_idx] = ft_strjoin("OLDPWD=", &env->data[idx][4]);
		else
			env->data[old_idx] = ft_strjoin("OLDPWD=", env->pwd_data);
	}
	cwd_wrapper(env, aim_dir);
	if (env->pwd_flag != -1)
		if (env->pwd_data && idx >= 0)
		{
			free(env->data[idx]);
			env->data[idx] = ft_strjoin("PWD=", env->pwd_data);
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
	env_home = get_aim_dir(env, &env->data[idx][5]);
	normalize(&env_home);
	if (chdir(env_home) == -1)
	{
		output_error("cd", strerror(ENOENT));
		free(env_home);
		return (EXIT_FAILURE);
	}
	cd_update_envpwd(env, env_home);
	free(env_home);
	return (EXIT_SUCCESS);
}

char	*get_aim_dir(t_env *env, char *cd_str)
{
	char *str;
	char *tmp;

	tmp = cwd_wrapper(env, NULL);
	if (*cd_str == '/' || !tmp || *tmp != '/')
		str = ft_strdup(cd_str);
	else
	{
		str = ft_strdup(tmp);
		if (str == NULL)
			str = ft_strdup("");
		if (ft_strlen(str) > 0 && *(str + ft_strlen(str) - 1) != '/')
			join_free(&str, "/");
		join_free(&str, cd_str);
	}
	return (str);
}

int		command_cd(t_token *token, t_env *env)
{
	char		*nom_path;

	token = token->next;
	if (token == NULL)
		return (cd_home(env));
	nom_path = get_aim_dir(env, token->data);
	normalize(&nom_path);
	printf("nom_path:[%s]\n", nom_path);
	if (chdir(nom_path) == -1)
	{
		error_str(strerror(errno));
		error_str(": ");
		if (check_cd(token->data))
			error_str(token->data);
		else
			fail_with_relativepath(env, token->data);
		output_error("cd", error_str(""));
		error_str(NULL);
		free(nom_path);
		return (EXIT_FAILURE);
	}
	if (*nom_path != '/')
		setup_relativepath(&nom_path, env, token->data);
	cd_update_envpwd(env, nom_path);
	free(nom_path);
	return (EXIT_SUCCESS);
}
