/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_search_cmdpath.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 02:49:44 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 03:09:08 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

DIR	*open_dir_path(char *dir_path)
{
	DIR	*open_dp;

	open_dp = opendir(dir_path);
	if (open_dp == NULL)
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	return (open_dp);
}

void	format_cmd_path(t_token *token, DIR *dp, char *dir_path)
{
	char	*tmp;

	tmp = ft_strjoin("/", token->data);
	free(token->data);
	token->data = ft_strjoin(dir_path, tmp);
	free(tmp);
	closedir(dp);
}

int	execve_search_cmdpath(t_token *token, t_env *env)
{
	struct dirent	*dirp;
	DIR				*dp;
	char			**path_value;
	int				idx;

	if (ft_strchr(token->data, '/'))
		return (1);
	idx = env_search(env->data, "PATH");
	if (idx == -1)
		return (1);
	path_value = ft_split(&env->data[idx][5], ':');
	idx = 0;
	while (path_value[idx] != NULL)
	{
		dp = open_dir_path(path_value[idx]);
		if (dp == NULL)
			return (0);
		while ((dirp = readdir(dp)) != NULL)
		{
			if (ft_strcmp(token->data, dirp->d_name) == 0)
			{
				format_cmd_path(token, dp, path_value[idx]);
				free_args(path_value);
				return (1);
			}
		}
		closedir(dp);
		idx++;
	}
	free_args(path_value);
	return (0);
}
