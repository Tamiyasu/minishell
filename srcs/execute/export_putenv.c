/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_putenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:11:32 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 02:12:43 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

void	putenv_free(char **shape_env)
{
	int	idx;

	idx = 0;
	while (shape_env[idx] != NULL)
	{
		if (ft_strcmp(shape_env[idx], "=\"\"") == 0)
		{
			free(shape_env[idx]);
			break ;
		}
		free(shape_env[idx]);
		idx++;
	}
	free(shape_env);
	shape_env = NULL;
}

void	set_idx_array(int *idx_array, t_env *env)
{
	int	i;

	i = 0;
	while (i < env->num)
	{
		idx_array[i] = i;
		i++;
	}
	idx_array[i] = '\0';
}

void	put_shape_env(char **shape_env)
{
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(shape_env[0], STDOUT_FILENO);
	if (shape_env[1] != NULL)
		ft_putstr_fd(shape_env[1], STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

int		export_putenv(t_env *env)
{
	char	**shape_env;
	int		*idx_array;
	int		i;

	idx_array = malloc(sizeof(int *) * (env->num + 1));
	if (idx_array == NULL)
		ft_enomem();
	set_idx_array(idx_array, env);
	export_sort(env->data, idx_array, env->num);
	i = 0;
	while (i < env->num)
	{
		shape_env = export_shape_putenv(env->data[idx_array[i]]);
		if (ft_strncmp(shape_env[0], "_", 2) == 0)
		{
			putenv_free(shape_env);
			i++;
			continue ;
		}
		put_shape_env(shape_env);
		putenv_free(shape_env);
		i++;
	}
	free(idx_array);
	return (EXIT_SUCCESS);
}
