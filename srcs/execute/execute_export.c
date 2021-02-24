/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 16:10:16 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/22 11:19:35 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "libft.h"

static void	execute_output_error(char *token_data)
{
	ft_putstr_fd("minishell: unset: `",  1);
	ft_putstr_fd(token_data, 1);
	ft_putendl_fd("': not a valid identifier", 1);
}

static int	msh_lstsize(t_lsttoken *token)
{
	int i;

	i = 0;
	while (token)
	{
		i++;
		token = token->next;
	}
	return (i);
}


static char	**execute_split_and_classify_tokend(t_lsttoken *token,  t_env *env)
{
	char	**split_tokend;
	char	**tmp_split;
	int		i;

	split_tokend = malloc(sizeof(char *) * (msh_lstsize(token) + 1));
	if (split_tokend == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (token != NULL)
	{
		if (token->data[0] == '=')
		{
			split_tokend[i] = ft_strdup("0");
			token->flag = -1;
		}
		else if (ft_strchr(token->data, '=') == 0)//'='がなかったら
		{
			split_tokend[i] = ft_strdup(token->data);
			token->flag = 1;
		}
		else
		{
			tmp_split = ft_split(token->data, '=');
			split_tokend[i] = ft_strdup(tmp_split[0]);
			free_args(tmp_split);
			token->flag = 2;
		}
		/* env->pwd_flag && oldpwd_flagの更新 */
		if (ft_strcmp(split_tokend[i], "PWD")== 0)
		{
			env->pwd_flag = 1;
		}
		if (ft_strcmp(split_tokend[i], "OLDPWD")== 0)
		{
			env->oldpwd_flag = 1;
		}
		i++;
		token = token->next;
	}
	split_tokend[i] = NULL;
	return (split_tokend);
}

static void	export_check_args(t_lsttoken *token, char **split_tokend, int *exit_status)
{
	int	i;
	int	j;

	i = 0;
	while (split_tokend[i] != NULL)
	{
		j = 0;
		while (split_tokend[i][j] != '\0')
		{
			if (token->flag == -1)
			{
				execute_output_error(token->data);
				*exit_status = 1;
				break ;
			}
			else if (j == 0)
			{
				if (ft_isalpha(split_tokend[i][j]) == 0 && split_tokend[i][j] != '_')
				{
					token->flag = -1;
					execute_output_error(token->data);
					*exit_status = 1;
					break ;
				}
			}
			else
			{
				if (ft_isalnum(split_tokend[i][j]) == 0 && split_tokend[i][j] != '_')
				{
					token->flag = -1;
					execute_output_error(token->data);
					*exit_status = 1;
					break ;
				}
			}
			j++;
		}
		i++;
		token = token->next;//token->data/token->flagとsplit_tokendが対応する様に動かす
	}
}

static int	check_duplication_token(char *fwd_tokend, char **bwd_tokend, t_lsttoken *token)
{
	int	save_flag;
	int	idx;

	save_flag = token->flag;
	idx = 0;

	token = token->next; //bwd_tokenとtoken->flagを対応させる。
	while (bwd_tokend[idx] !=  NULL)
	{
		if (token->flag != -1) /*比較する*/
		{
			if (ft_strcmp(fwd_tokend, bwd_tokend[idx]) == 0)
			{
				if (save_flag <= token->flag) /* flag1,2はflag2に上書きされる。flag2はflag1に上書きされない */
				{
					return (-1);
				}
			}
		}
		idx++;
		token = token->next;
	}
	return (save_flag);
}

static void	export_check_duplication_of_token(t_lsttoken *token, char **split_tokend)
{
	int	idx;

	idx = 0;
	while (split_tokend[idx + 1] != NULL)
	{
		token->flag = check_duplication_token(split_tokend[idx], &split_tokend[idx + 1], token);
		token = token->next;
		idx++;
	}
}

static void	export_compare_args_with_env(t_lsttoken *token, char **split_tokend, t_env *env, char **split_env)
{
	int	idx;
	int	envidx;

	idx = 0;
	while (split_tokend[idx] != NULL)
	{
		envidx = 0;
		if (token->flag == 1)
		{
			while (split_env[envidx] != NULL)
			{
				if (ft_strcmp(split_tokend[idx], split_env[envidx]) == 0)/*上書きできない*/
				{
					token->flag = -1;
					break ;
				}
				envidx++;
			}
			if (split_env[envidx] ==  NULL) /* 同じ名前の環境変数がなかったので、新しく追加される*/
			{
				env->num++;
			}
		}
		else if (token->flag == 2)
		{
			while (split_env[envidx] != NULL)
			{
				if (ft_strcmp(split_tokend[idx], split_env[envidx]) == 0)
				{
					free(env->data[envidx]);
					env->data[envidx] = ft_strdup(token->data);
					token->flag = 0;
					break ;
				}
				envidx++;
			}
			if (split_env[envidx] ==  NULL) /* 同じ名前の環境変数がなかったので、新しく追加される*/
			{
				env->num++;
			}
		}
		idx++;
		token = token->next;
	}
}

static void	export_make_new_envdata(t_lsttoken *token, t_env *env)
{
	char	**new_env;
	int		idx;

	new_env = malloc(sizeof(char *) * (env->num + 1));
	if (new_env == NULL)
	{
		return ;
	}
	idx = 0;
	while (env->data[idx] != NULL)
	{
		new_env[idx] = ft_strdup(env->data[idx]);
		idx++;
	}
	while(token != NULL)
	{
		if (ft_strncmp(token->data, "PWD", 3) == 0 && token->data[3] == '\0' && token->flag == 1)
		{
			if (ft_strcmp(env->pwd_data,  env->unset_pwd) != 0) //PWD -> PWD=cwdirにする
			{
				free(token->data);
				token->data = ft_strjoin("PWD=", env->pwd_data);
			}
		}
		if (token->flag > 0)/* 新しく追加されるやつ */
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

int			execute_export(t_lsttoken *token, t_env *env)
{
	char	**split_tokend;
	char	**split_env;
	int		exit_status;

	exit_status = 0;
	token = token->next;
	if (token == NULL) /* exportのみ --->ascii順に環境変数表示*/
	{
		exit_status = export_putenv(env);
		return (exit_status);
	}
	split_tokend = execute_split_and_classify_tokend(token, env);
	export_check_args(token, split_tokend, &exit_status);
	export_check_duplication_of_token(token, split_tokend);
	split_env = execute_split_env(env);
	if (split_env == NULL)//execute_split_env()内でmallocエラー
	{
		ft_putendl_fd(strerror(errno), 1);
		exit_status = 1;
		return (exit_status);
	}
	export_compare_args_with_env(token, split_tokend, env, split_env);
	export_make_new_envdata(token, env);
	free_args(split_tokend);
	free_args(split_env);
	return (exit_status);
}
