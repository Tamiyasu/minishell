/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmp_args.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 01:06:54 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 01:23:16 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	cmp_case_flag1(t_token *token, t_env *env,
						char **split_env, char *split_tokend)
{
	int	envidx;

	envidx = 0;
	while (split_env[envidx] != NULL)
	{
		if (ft_strcmp(split_tokend, split_env[envidx]) == 0)
		{
			token->flag = -1;
			break ;
		}
		envidx++;
	}
	if (split_env[envidx] == NULL)
		env->num++;
}

void	cmp_case_flag2(t_token *token, t_env *env,
						char **split_env, char *split_tokend)
{
	int	envidx;

	envidx = 0;
	while (split_env[envidx] != NULL)
	{
		if (ft_strcmp(split_tokend, split_env[envidx]) == 0)
		{
			free(env->data[envidx]);
			env->data[envidx] = ft_strdup(token->data);
			token->flag = 0;
			break ;
		}
		envidx++;
	}
	if (split_env[envidx] == NULL)
		env->num++;
}

void	export_cmp_args(t_token *token, char **split_tokend,
						t_env *env, char **split_env)
{
	int	idx;

	idx = 0;
	while (split_tokend[idx] != NULL)
	{
		if (token->flag == 1)
			cmp_case_flag1(token, env, split_env, split_tokend[idx]);
		else if (token->flag == 2)
			cmp_case_flag2(token, env, split_env, split_tokend[idx]);
		idx++;
		token = token->next;
	}
}
