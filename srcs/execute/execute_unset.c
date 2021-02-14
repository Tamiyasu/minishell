/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 15:17:00 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/08 17:48:15 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "libft.h"

static void	unset_check_args(t_lsttoken *token)
{
	int	idx;

	while (token != NULL)
	{
		idx = 0;
		while (token->data[idx] != '\0')
		{
			if (idx == 0)
			{
				if (ft_isalpha(token->data[idx]) == 0 && token->data[idx] != '_')
				{
					token->flag = -1;
					ft_putstr_fd("minishell: unset: `",  1);
					ft_putstr_fd(token->data, 1);
					ft_putendl_fd("': not a valid identifier", 1);
					break ;
				}
			}
			else
			{
				if (ft_isalnum(token->data[idx]) == 0 && token->data[idx] != '_')
				{
					token->flag = -1;
					ft_putstr_fd("minishell: unset: `",  1);
					ft_putstr_fd(token->data, 1);
					ft_putendl_fd("': not a valid identifier", 1);
					break ;
				}
			}
			idx++;
		}
		token = token->next;
	}
}

static void	unset_compare_args_with_env(t_lsttoken *token, t_env *env, char **split_env)
{
	char	**split_tokend;
	int		idx;

	while (token != NULL)
	{
		if (token->flag == -1)
		{
			token  = token->next;
			continue ;
		}
		split_tokend = ft_split(token->data, '=');
		idx = 0;
		while (split_env[idx] != NULL)
		{
			if (split_env[idx][0] == '0')
			{
				idx++;
				continue ;
			}
			if (ft_strcmp(split_env[idx], split_tokend[0]) == 0)/*一致していたら*/
			{
				env->num--;
				free(split_env[idx]);
				split_env[idx] = ft_strdup("0");/*消すという印*/
			}
			idx++;
		}
		free_args(split_tokend);
		token = token->next;
	}
}

static void	unset_make_new_envdata(t_env *env, char **split_env)
{
	char	**new_env;
	int		idx;

	new_env = malloc(sizeof(char *) * (env->num + 1));
	if (new_env == NULL)
	{
		return ;
	}
	idx = 0;
	int	new_idx = 0;;
	while (split_env[idx] != NULL)
	{
		if (split_env[idx][0] != '0')
		{
			new_env[new_idx] = ft_strdup(env->data[idx]);
			new_idx++;
		}
		idx++;
	}
	new_env[new_idx] = NULL;
	free_args(env->data);
	env->data = new_env;
}


void		execute_unset(t_lsttoken *token, t_env *env)
{
	char	**split_env;

	token = token->next;
	if (token == NULL)
	{
		return ;
	}
	unset_check_args(token);
	split_env = execute_split_env(env);
	if (split_env == NULL)
	{
		return ;
	}
	unset_compare_args_with_env(token, env, split_env);
	unset_make_new_envdata(env, split_env);
	free_args(split_env);
}
