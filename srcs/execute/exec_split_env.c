/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_split_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 19:44:14 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 22:59:29 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char **exec_split_env(t_env *env)
{
	char	**split_env;
	int		i;
	int		j;

	split_env = malloc(sizeof(char *) * (env->num + 1));
	if (split_env == NULL)
		ft_enomem();
	i = 0;
	while (env->data[i] != NULL)
	{
		j = 0;
		while (env->data[i][j] != '\0' && env->data[i][j] != '=')
		{
			j++;
		}
		split_env[i] = ft_substr(env->data[i], 0, j);
		i++;
	}
	split_env[i] = NULL;
	return (split_env);
}
