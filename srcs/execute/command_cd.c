/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:41:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/04 10:44:19 by tmurakam         ###   ########.fr       */
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

int		arr_size(char **null_ended_strs)
{
	int ret;

	ret = 0;
	while (*null_ended_strs++)
		ret++;
	return (ret);
}

char	*strs_join(char **strs, char *enc, char *f)
{
	char *str;
	char *tmp;

	str = ft_strdup(f);
	if (*strs)
	{
		tmp = str;
		str = ft_strjoin(tmp, *(strs++));
		free(tmp);
	}
	while (*strs)
	{
		tmp = str;
		str = ft_strjoin(tmp, enc);
		free(tmp);
		tmp = str;
		str = ft_strjoin(tmp, *(strs++));
		free(tmp);
	}
	return (str);
}

void	join_free(char **s1, char *s2)
{
	char *tmp;

	tmp = *s1;
	*s1 = ft_strjoin(*s1, s2);
	free(tmp);
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

int		check_cd(char *cd_str)
{
	char	**splited;
	int		i;

	i = 0;
	splited = ft_split(cd_str, '/');
	while (*(splited + i))
	{
		if (2 < ft_strlen(*(splited + i)) ||
			(ft_strcmp("..", *(splited + i)) != 0 &&
			ft_strcmp(".", *(splited + i)) != 0 &&
			ft_strcmp("", *(splited + i)) != 0))
			return (1);
		i++;
	}
	free_args(splited);
	return (0);
}

void	free_set(char **s1, char *s2)
{
	free(*s1);
	if (s2)
		*s1 = ft_strdup(s2);
	else
		*s1 = NULL;
}

void	path_recon(char **cds, char *f, char **cds_normed)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (*(cds + j))
	{
		if (!ft_strcmp(*(cds + j), "..") && i > 0
			&& ft_strcmp(*(cds_normed + i - 1), ".."))
		{
			i--;
			if (!ft_strcmp(*(cds_normed + i), "."))
				free_set(cds_normed + i, *(cds + j));
			else
				free_set(cds_normed + i, NULL);
		}
		else if ((!ft_strcmp(*(cds + j), "..")
			|| !ft_strcmp(*(cds + j), ".")) && *f == '/' && i == 0)
			;
		else if (i == 0 || ft_strcmp(*(cds + j), "."))
			*(cds_normed + i++) = ft_strdup(*(cds + j));
		j++;
	}
}

void	normalize(char **aim_dir)
{
	char	**cds;
	char	**cds_normed;
	char	*f;

	cds = ft_split(*aim_dir, '/');
	if (!*cds)
	{
		free_args(cds);
		return ;
	}
	f = (**aim_dir == '/' ? "/" : "");
	cds_normed = ft_calloc(sizeof(char *), arr_size(cds) + 1);
	free(*aim_dir);
	path_recon(cds, f, cds_normed);
	/*
	i = 0;
	j = 0;
	while (*(cds + j))
	{
		if (!ft_strcmp(*(cds + j), "..") && i > 0
			&& ft_strcmp(*(cds_normed + i - 1), ".."))
		{
			i--;
			if (!ft_strcmp(*(cds_normed + i), "."))
				free_set(cds_normed + i, *(cds + j));
			else
				free_set(cds_normed + i, NULL);
		}
		else if ((!ft_strcmp(*(cds + j), "..")
			|| !ft_strcmp(*(cds + j), ".")) && *f == '/' && i == 0)
			;
		else if (i == 0 || ft_strcmp(*(cds + j), "."))
			*(cds_normed + i++) = ft_strdup(*(cds + j));
		j++;
	}
	*/
	*aim_dir = strs_join(cds_normed, "/", f);
	free_args(cds);
	free_args(cds_normed);
}

void	setup_relativepath(char **path, t_env *env, char *cd_str)
{
	free(*path);
	if (env->pwd_data)
		*path = ft_strjoin(env->pwd_data, "/");
	else
		*path = ft_strdup("");
	join_free(path, cd_str);
	normalize(path);
}

void	fail_with_relativepath(t_env *env, char *cd_str)
{
	char *aim_dir;

	aim_dir = get_aim_dir(env, cd_str);
	error_str("cannot access parent directories");
	error_str("error retrieving current directory: getcwd: ");
	cd_update_envpwd(env, aim_dir);
	free(aim_dir);
}

int		command_cd(t_token *token, t_env *env)
{
	char		*nom_path;

	token = token->next;
	if (token == NULL)
		return (cd_home(env));
	nom_path = get_aim_dir(env, token->data);
	normalize(&nom_path);
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
