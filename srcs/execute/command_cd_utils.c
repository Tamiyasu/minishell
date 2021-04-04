/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/04 10:54:16 by tmurakam          #+#    #+#             */
/*   Updated: 2021/04/04 10:55:49 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "lexer.h"
#include "libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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
