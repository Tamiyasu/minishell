/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 16:10:16 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 16:02:22 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "execute.h"

void	export_check_args(t_token *token, char **split_tokend, int *status)
{
	int	i;
	int	j;

	i = 0;
	while (split_tokend[i] != NULL)
	{
		j = 0;
		while (split_tokend[i][j] != '\0')
		{
			if ((token->flag == -1)
				|| (j == 0 && (!ft_isalpha(split_tokend[i][j])
					&& split_tokend[i][j] != '_'))
				|| (j != 0 && (!ft_isalnum(split_tokend[i][j])
					&& split_tokend[i][j] != '_')))
			{
				unset_error(token, "export", split_tokend[i], status);
				break ;
			}
			j++;
		}
		i++;
		token = token->next;
	}
}

void	set_pwd(t_token *token, t_env *env)
{
	if (ft_strncmp(token->data, "PWD", 3) == 0
		&& token->data[3] == '\0' && token->flag == 1)
	{
		if (ft_strcmp(env->pwd_data, env->unset_pwd) != 0)
		{
			free(token->data);
			token->data = ft_strjoin("PWD=", env->pwd_data);
		}
	}
	else if (ft_strcmp(token->data, "OLDPWD") == 0 && token->flag == 1)
	{
		if (env->oldpwd_data)
		{
			free(token->data);
			token->data = ft_strjoin("OLDPWD=", env->oldpwd_data);
		}
	}
}

void	export_make_new_envdata(t_token *token, t_env *env)
{
	char	**new_env;
	int		idx;

	new_env = malloc(sizeof(char *) * (env->num + 1));
	if (new_env == NULL)
		ft_enomem();
	idx = 0;
	while (env->data[idx] != NULL)
	{
		new_env[idx] = ft_strdup(env->data[idx]);
		idx++;
	}
	while (token != NULL)
	{
		set_pwd(token, env);
		if (token->flag > 0)
		{
			new_env[idx] = ft_strdup(token->data);
			idx++;
		}
		token = token->next;
	}
	new_env[idx] = NULL;
	free_args(env->data);
	env->data = new_env;
}

int		command_export(t_token *token, t_env *env)
{
	char	**split_tokend;
	char	**split_env;
	int		status;

	status = 0;
	token = token->next;
	if (token == NULL)
	{
		status = export_putenv(env);
		return (status);
	}
	split_tokend = export_format_variable(token, env);
	export_check_args(token, split_tokend, &status);
	export_check_dupl(token, split_tokend);
	split_env = exec_split_env(env);
	export_cmp_args(token, split_tokend, env, split_env);
	export_make_new_envdata(token, env);
	free_args(split_tokend);
	free_args(split_env);
	return (status);
}
