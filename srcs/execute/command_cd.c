/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:41:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/01 01:05:15 by tmurakam         ###   ########.fr       */
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

int		arr_size(char **null_ended_strs)
{
	int ret;
	ret = 0;
	while (*null_ended_strs++)
		ret++;
	return (ret);
}

char	*strs_join(char **strs, char *enc)
{
	char *str;
	char *tmp;

	str = ft_strjoin("", "");
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

char	*get_aim_dir(t_env *env, char *cd_str)
{
	char	**cds;
	char	**a_cds;
	char	*str;
	int		size;
	int		i;
	int		j;
	char	**e_cds;

	str = cwd_wrapper(env, NULL);
	cds = ft_split(str, '/');
	a_cds = ft_split(cd_str, '/');
	size = arr_size(cds) + arr_size(a_cds);
	e_cds = ft_calloc(sizeof(char *), size + 1);
	i = 0;
	while(*(cds + i))
	{
		*(e_cds + i) = ft_strdup(*(cds + i));
		i++;
	}
	j = 0;
	while(*(a_cds + j))
	{
		if	(!ft_strcmp(*(a_cds + j), ".."))
		{
			printf("----------\n");
			i--;
			free(*(e_cds + i));
			*(e_cds + i) = NULL;
			j++;
		}
		else if (!ft_strcmp(*(a_cds + j), "."))
		{
			j++;
		}
		else
		{
			*(e_cds + i) = ft_strdup(*(a_cds + j));
			i++;
			j++;
		}
	}
	str = strs_join(e_cds, "/");
	printf("%s\n", str);
	free_args(cds);
	free_args(a_cds);
	free_args(e_cds);
	return (str);
}


int		command_cd(t_token *token, t_env *env)
{
	char		*err_str;
	char		*aim_dir;
	//struct stat	stat_buf;
	//int			err_cord;

	token = token->next;
	if (token == NULL)
		return (cd_home(env));
	aim_dir = get_aim_dir(env, token->data);
	if (chdir(aim_dir) == -1)
	{
		//err_cord = ENOENT;
		// if (!stat(token->data, &stat_buf)
		// 	&& (stat_buf.st_mode & S_IFMT) != S_IFDIR)
		// 	err_cord = ENOTDIR;
		err_str = ft_strjoin("cd: ", token->data);
		output_error(err_str, strerror(/*err_cord*/errno));
		free(err_str);
		return (EXIT_FAILURE);
	}
	free(aim_dir);
	cd_update_envpwd(env, token->data);
	return (EXIT_SUCCESS);
}
