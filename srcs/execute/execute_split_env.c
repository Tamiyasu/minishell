/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_split_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/06 19:44:14 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 22:41:40 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char **execute_split_env(t_env *env)
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
		while (env->data[i][j] != '\0' && env->data[i][j] != '=')/*=がない時はあるか*/
		{
			j++;
		}
		split_env[i] = ft_substr(env->data[i], 0, j); /* '=' の前まで切り取って保存*/
		i++;
	}
	split_env[i] = NULL;
	return (split_env);
}
