/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/05 15:17:00 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/06 20:23:31 by ysaito           ###   ########.fr       */
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

// static char **unset_split_env(t_env *env)
// {
// 	char	**split_env;
// 	int		i;
// 	int		j;

// 	split_env = malloc(sizeof(char *) * (env->num + 1));
// 	if (split_env == NULL)
// 	{
// 		return (NULL);
// 	}
// 	i = 0;
// 	while (env->data[i] != NULL)
// 	{
// 		j = 0;
// 		while (env->data[i][j] != '=')/*=がない時はあるか*/
// 		{
// 			j++;
// 		}
// 		split_env[i] = ft_substr(env->data[i], 0, j); /* '=' の前まで切り取って保存*/
// 		i++;
// 	}
// 	split_env[i] = NULL;
// 	return (split_env);
// }

static void	unset_compare_args_with_env(t_lsttoken *token, t_env *env, char **split_env)
{
	char	**split_tokend;
	int		idx;

	while (token != NULL)
	{
		if (ft_strlen(token->data) == 0) /* これ書いた時(2021/2/5)parseが完成していない->最後にspaceを入れられるとそれがtokenに入ってくる->splitすると(null)が入る->次にft_strcmpでSEGV　Abort trap6 になる.そのための例外処理parseで対応するならこの条件消す. */
		{
			token = token->next;
			continue ;
		}
		if (token->flag == -1)
		{
			// ft_putstr_fd("minishell: unset: `",  1);
			// ft_putstr_fd(token->data, 1);
			// ft_putendl_fd("': not a valid identifier", 1);
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
	// printf("before num=[%d]\n", env->num);//del

	token = token->next;
/* unsetコマンドのみ入力された→何も起こらずプロンプト再表示 */
	if (token == NULL)
	{
		return ;
	}
/* 1.unsetの引数(消したい変数名)をチェック */
	//if (使えない文字を含む == 識別子エラー)
	//	token->flagに-1を入れてエラーの印とする。
	unset_check_args(token);
/* 2.envを'='でsplit*/
	split_env = execute_split_env(env);
	if (split_env == NULL)
	{
		return ;
	}
/* 4.split_env[0]とtoken->data比較*/
	//if (flag == -1)はスキップ
	//if (同じ変数名があったら、'0'を印として入れる)
	unset_compare_args_with_env(token, env, split_env);
	unset_make_new_envdata(env, split_env);
	free_args(split_env);
}
