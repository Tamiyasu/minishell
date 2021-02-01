/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:41:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/01 16:39:02 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

void	execute_cd(t_lsttoken *token, char **env_data)
{
	int		idx;
	char	**split_env;

	token = token->next;
	idx = 0;
	if (token == NULL) /* cdのみ -> 環境変数のHOMEに指定されているpathに移動 */
	{
		while (env_data[idx] != NULL)
		{
			split_env = NULL;
			split_env = ft_split(env_data[idx], '=');
			if (ft_strncmp(split_env[0], "HOME", 5) == 0)
			{
				break;
			}
			free_args(split_env);
			idx++;
		}
		if (chdir(split_env[1]) == -1) /* HOME変数が無い時 -> エラー出力 */
		{
			ft_putstr_fd("minishell: cd: ", 1);
			ft_putstr_fd(split_env[1], 1);
			ft_putendl_fd(": No such file or directory", 1);
		}
		free_args(split_env);
		return;
	}
	if (chdir(token->data) == -1) /* chdir()のエラー時 */
	{
		ft_putstr_fd("minishell: cd: ", 1);
		ft_putstr_fd(token->data, 1);
		ft_putendl_fd(": No such file or directory", 1);
	}
}
