/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:41:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/21 19:43:10 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "lexer.h"
#include "libft.h"

static void	cd_output_error(char *str)
{
	ft_putstr_fd("minishell: cd: ", 1);
	ft_putstr_fd(str, 1);
	ft_putendl_fd(": No such file or directory", 1);
}

static void	cd_update_envpwd(t_env *env)
{
	int	old_idx;
	int	idx;

	idx = msh_env_search(env->data, "PWD");
	if (env->oldpwd_flag != -1) /* OLDPWDの更新*/
	{
		old_idx = msh_env_search(env->data, "OLDPWD");
		free(env->data[old_idx]);
		if (env->pwd_flag != -1)
		{
			env->data[old_idx] = ft_strjoin("OLDPWD=", &env->data[idx][4]);
		}
		else
		{
			env->data[old_idx] = ft_strjoin("OLDPWD=", env->pwd_data);
		}
	}
	msh_env_update_pwddata(env); /*env->pwd_dataの更新*/
	if  (env->pwd_flag != -1)
	{
		free(env->data[idx]);
		env->data[idx] = ft_strjoin("PWD=", env->pwd_data);
	}
}

int	execute_cd(t_lsttoken *token, t_env *env)
{
	char	**split_env;
	int		idx;

	token = token->next;
	if (token == NULL) /* cdのみ -> 環境変数のHOMEに指定されているpathに移動*/
	{
		idx = msh_env_search(env->data, "HOME");
		if (idx == -1)  /* 環境変数HOMEが存在しない時 */
		{
			ft_putendl_fd("minishell: cd: HOME not set", 1);
			return (1);
		}
		split_env = ft_split(env->data[idx], '=');
		if (chdir(split_env[1]) == -1) /* 環境変数HOMEの値が存在しないpathだった時 */
		{
			cd_output_error(split_env[1]);
			free_args(split_env);
			return (1);
		}
		free_args(split_env);
	}
	else if (chdir(token->data) == -1)
	{
		cd_output_error(token->data);
		return (1);
	}
	cd_update_envpwd(env);
	return (0);
}
