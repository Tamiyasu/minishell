/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_format_variable.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 00:32:23 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 01:24:04 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	set_split_tokend(t_token *token, char **split_tokend,
						char *set_token, int flag)
{
	*split_tokend = ft_strdup(set_token);
	token->flag = flag;
}

void	set_split_format(t_token *token, char **split_tokend, int idx)
{
	char	**tmp_split;

	tmp_split = ft_split(token->data, '=');
	split_tokend[idx] = ft_strdup(tmp_split[0]);
	free_args(tmp_split);
	token->flag = 2;
}

char	**export_format_variable(t_token *token, t_env *env)
{
	char	**split_tokend;
	int		i;

	split_tokend = malloc(sizeof(char *) * (token_lstsize(token) + 1));
	if (split_tokend == NULL)
		ft_enomem();
	i = 0;
	while (token != NULL)
	{
		if (token->data[0] == '=')
			set_split_tokend(token, &split_tokend[i], token->data, -1);
		else if (!ft_strchr(token->data, '='))
			set_split_tokend(token, &split_tokend[i], token->data, 1);
		else
			set_split_format(token, split_tokend, i);
		if (ft_strcmp(split_tokend[i], "PWD") == 0)
			env->pwd_flag = 1;
		if (ft_strcmp(split_tokend[i], "OLDPWD") == 0)
			env->oldpwd_flag = 1;
		i++;
		token = token->next;
	}
	split_tokend[i] = NULL;
	return (split_tokend);
}
