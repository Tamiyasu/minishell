/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_search_cmdpath.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 02:49:44 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 16:21:32 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	format_cmd_path(t_token *token, DIR *dp, char *dir_path)
{
	char	*tmp;

	tmp = ft_strjoin("/", token->data);
	free(token->data);
	token->data = ft_strjoin(dir_path, tmp);
	free(tmp);
	closedir(dp);
}

int		set_found_commandpath(t_token *token,
			DIR *dp, char **path_value, int idx)
{
	format_cmd_path(token, dp, path_value[idx]);
	free_args(path_value);
	return (1);
}

int		execve_search_cmdpath(t_token *token, t_env *env)
{
	struct dirent	*dirp;
	DIR				*dp;
	char			**path_value;
	int				idx;

	if (ft_strchr(token->data, '/'))
		return (1);
	if ((idx = env_search(env->data, "PATH")) == -1)
		return (1);
	path_value = ft_split(&env->data[idx][5], ':');
	idx = 0;
	while (path_value[idx] != NULL)
	{
		dp = opendir(path_value[idx]);
		if (dp != NULL)
		{
			while ((dirp = readdir(dp)) != NULL)
				if (ft_strcmp(token->data, dirp->d_name) == 0)
					return (set_found_commandpath(token, dp, path_value, idx));
			closedir(dp);
		}
		idx++;
	}
	free_args(path_value);
	return (0);
}
