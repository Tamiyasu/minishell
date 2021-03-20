/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_shape_putenv.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 02:00:18 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 02:12:13 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int		increment_to_equal(char *env)
{
	int	idx;

	idx = 0;
	while (env[idx] != '\0')
	{
		if (env[idx] == '=')
			break ;
		idx++;
	}
	return (idx);
}

void	set_shape_env(char **shape_env, char *str)
{
	shape_env[1] = str;
	shape_env[2] = NULL;
}

char	**export_shape_putenv(char *env)
{
	char	**shape_env;
	char	*tmp;
	int		idx;

	shape_env = malloc(sizeof(char *) * 3);
	if (!shape_env)
		ft_enomem();
	idx = increment_to_equal(env);
	shape_env[0] = ft_substr(env, 0, idx);
	if (env[idx] == '\0')
	{
		shape_env[1] = NULL;
		return (shape_env);
	}
	idx++;
	if (env[idx] == '\0')
	{
		set_shape_env(shape_env, ft_strdup("=\"\""));
		return (shape_env);
	}
	tmp = ft_strjoin("=\"", &env[idx]);
	set_shape_env(shape_env, ft_strjoin(tmp, "\""));
	free(tmp);
	return (shape_env);
}
