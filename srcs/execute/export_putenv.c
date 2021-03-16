/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_putenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/03 18:11:32 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/16 17:21:11 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

static void	putenv_free(char **shape_env)
{
	int	idx;

	idx = 0;
	while (shape_env[idx] != NULL)
	{
		if (ft_strcmp(shape_env[idx], "=\"\"") == 0)
		{
			free(shape_env[idx]);
			break;
		}
		free(shape_env[idx]);
		idx++;
	}
	free(shape_env);
	shape_env = NULL;
}

/*
** 環境変数envpをindex_numを使ってsort
*/
static void	putenv_sort(char **envp, int *idx, int num)
{
	int i;

	i = 0;
	int loop_count = num;
	int	sort_count;
	int	left;
	int	right;
	int	tmp;
	while (loop_count)
	{
		left = (num - 2);
		right = (num - 1);
		sort_count = 1;
		while (sort_count < loop_count)
		{
			i = 0;
			while (envp[idx[left]][i] == envp[idx[right]][i])
			{
				i++;
			}
			if (envp[idx[left]][i] > envp[idx[right]][i])
			{
				tmp = idx[left];
				idx[left] = idx[right];
				idx[right] = tmp;
			}
			sort_count++;
			left--;
			right--;
		}
		loop_count--;
	}
}

static char	**putenv_shape(char *env)
{
	char	**shape_env;
	char	*tmp;
	int		idx;

	shape_env = malloc(sizeof(char *) * 3);
	idx = 0;
	while (env[idx] != '\0')
	{
		if (env[idx] == '=')
			break ;
		idx++;
	}
	shape_env[0] = ft_substr(env, 0, idx);
	//もし'='がなかったら
	if (env[idx] == '\0')
	{
		shape_env[1] = NULL;
		return (shape_env);
	}
	idx++;
	//もし'='の後ろに値がなかったら
	if (env[idx] == '\0')
	{
		shape_env[1] = ft_strdup("=\"\"");
		shape_env[2] = NULL;
		return (shape_env);
	}
	tmp = ft_strjoin("=\"", &env[idx]);
	shape_env[1] = ft_strjoin(tmp, "\"");
	shape_env[2] = NULL;
	free(tmp);
	return (shape_env);
}

int		export_putenv(t_env *env)
{
	char	**shape_env;
	int		*idx_array;
	int		i;

	idx_array = malloc(sizeof(int *) * (env->num + 1));
	if (idx_array == NULL)
	{
		ft_putendl_fd(strerror(errno), 1);//mallocのエラー出力して?return(最初の入力待ち)に
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < env->num)
	{
		idx_array[i] = i;
		i++;
	}
	idx_array[i] = '\0';
	putenv_sort(env->data, idx_array, env->num);
	i = 0;
	while (i < env->num)/* 環境変数を一つずつ出力形式に合わせて整形 */
	{
		shape_env = putenv_shape(env->data[idx_array[i]]);
		if (ft_strncmp(shape_env[0], "_", 2) == 0)
		{
			putenv_free(shape_env);
			i++;
			continue ;
		}
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(shape_env[0], 1);
		if (shape_env[1] != NULL)
		{
			ft_putstr_fd(shape_env[1], 1);
		}
		ft_putchar_fd('\n', 1);
		putenv_free(shape_env);
		i++;
	}
	free(idx_array);
	idx_array = NULL;
	return (EXIT_SUCCESS);
}
