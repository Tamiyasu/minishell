/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 15:17:00 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 22:46:40 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "libft.h"

void	unset_check_args(t_token *token, int *status)
{
	int	idx;

	while (token != NULL)
	{
		idx = 0;
		while (token->data[idx] != '\0')
		{
			if (idx == 0 &&
				(!ft_isalpha(token->data[idx]) && token->data[idx] != '_'))
			{
				unset_error(token, status);
				break ;
			}
			else if (!ft_isalnum(token->data[idx]) && token->data[idx] != '_')
			{
				unset_error(token, status);
				break ;
			}
			idx++;
		}
		token = token->next;
	}
}

void	unset_check_pwd(char *token_data, t_env *env)
{
	char *cwdir;

	if (ft_strcmp(token_data, "PWD") == 0)
	{
		env->pwd_flag = -1;
		if (env->pwd_data != NULL)
			free(env->pwd_data);
		env->pwd_data = ft_strdup("");
		if (env->unset_pwd != NULL)
		{
			free(env->unset_pwd);
			env->unset_pwd = NULL;
		}
		cwdir = getcwd(NULL, 0);
		if (cwdir == NULL)
		{
			strerror(errno);
			return ;
		}
		env->unset_pwd = ft_strdup(cwdir);
		free(cwdir);
	}
	else if (ft_strcmp(token_data, "OLDPWD") == 0)
		env->oldpwd_flag = -1;
}

void	unset_cmp_token_with_env(t_token *token, t_env *env, char **split_env)
{
	int		idx;

	while (token != NULL)
	{
		if (token->flag == -1)
		{
			token = token->next;
			continue ;
		}
		unset_check_pwd(token->data, env);
		idx = 0;
		while (split_env[idx] != NULL)
		{
			if (split_env[idx][0] == '0')
			{
				idx++;
				continue ;
			}
			if (ft_strcmp(split_env[idx], token->data) == 0)
			{
				env->num--;
				free(split_env[idx]);
				split_env[idx] = ft_strdup("0");
			}
			idx++;
		}
		token = token->next;
	}
}

void	unset_make_new_envdata(t_env *env, char **split_env)
{
	char	**new_env;
	int		new_idx;
	int		idx;

	new_env = malloc(sizeof(char *) * (env->num + 1));
	if (new_env == NULL)
		return ;
	idx = 0;
	new_idx = 0;
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

int		command_unset(t_token *token, t_env *env)
{
	char	**split_env;
	int		status;

	token = token->next;
	status = 0;
	if (token == NULL)
		return (status);
	unset_check_args(token, &status);
	split_env = exec_split_env(env);
	if (split_env == NULL)//malloc失敗時
		return (EXIT_FAILURE);
	unset_cmp_token_with_env(token, env, split_env);
	unset_make_new_envdata(env, split_env);
	free_args(split_env);
	return (status);
}
