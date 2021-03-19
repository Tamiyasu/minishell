/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 15:17:00 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 16:54:20 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "libft.h"

// static void	unset_output_error(char *token_data)
// {
// 	ft_putstr_fd("minishell: unset: `",  STDERR_FILENO);
// 	ft_putstr_fd(token_data, STDERR_FILENO);
// 	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
// }

/*
** 指定された変数名[0]が文字or'_'で始まっているか、かつ、使用していい文字の変数名かチェック。
** 変数名にエラーあり→token->flag = -1代入し、エラー文出力。
*/
static void	unset_check_args(t_lsttoken *token, int *g_exit_status)
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
					//unset_output_error(token->data);
					output_error("unset", "not a valid identifier");
					*g_exit_status = 1;
					break ;
				}
			}
			else
			{
				if (ft_isalnum(token->data[idx]) == 0 && token->data[idx] != '_')
				{
					token->flag = -1;
					//unset_output_error(token->data);
					output_error("unset", "not a valid identifier");
					*g_exit_status = 1;
					break ;
				}
			}
			idx++;
		}
		token = token->next;
	}
}

static void	unset_check_pwd(char *token_data, t_env *env)
{
	if (ft_strcmp(token_data, "PWD") == 0)  /* unset PWDの時 */
	{
		env->pwd_flag = -1;
		if (env->pwd_data != NULL)
		{
			free(env->pwd_data);
		}
		env->pwd_data = ft_strdup("");


		char *cwdir;
		if (env->unset_pwd != NULL)
		{
			free(env->unset_pwd);
			env->unset_pwd = NULL;
		}
		cwdir = getcwd(NULL, 0);//このように引数を指定してあげると、getcwdの方で、buf[1024]で取ってくれる。
		if (cwdir == NULL)
		{
			strerror(errno);
			return ;
		}
		env->unset_pwd = ft_strdup(cwdir);
		free(cwdir);


	}
	else if (ft_strcmp(token_data, "OLDPWD") == 0) /* unset OLDPWDの時 */
	{
		env->oldpwd_flag = -1;
	}
}

static void	unset_compare_token_with_env(t_token *token, t_env *env, char **split_env)
{
	int		idx;

	while (token != NULL)
	{
		if (token->flag == -1)
		{
			token  = token->next;
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
			if (ft_strcmp(split_env[idx], token->data) == 0)  /* 一致していたら */
			{
				env->num--;
				free(split_env[idx]);
				split_env[idx] = ft_strdup("0"); /* 消すという印 */
			}
			idx++;
		}
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


int		execute_unset(t_token *token, t_env *env)
{
	char	**split_env;
	int		g_exit_status;

	token = token->next;
	g_exit_status = 0;
	if (token == NULL)
	{
		return (g_exit_status);
	}
	unset_check_args(token,  &g_exit_status);
	split_env = execute_split_env(env);
	if (split_env == NULL) //malloc失敗時
	{
		return (EXIT_FAILURE);
	}
	unset_compare_token_with_env(token, env, split_env);
	unset_make_new_envdata(env, split_env);
	free_args(split_env);
	return (g_exit_status);
}
