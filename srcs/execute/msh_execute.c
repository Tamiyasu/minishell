/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/28 19:48:45 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

/*
** コマンドを実行する　returnで終了ステータスを返す.
*/
int	msh_execute(t_lsttoken *token, t_env *env, int *exit_status)
{
	printf("in msh_execute token : %p\n", token);
	if (ft_strcmp(token->data, "cd") == 0)
	{
		execute_cd(token, env);
	}
	else if (ft_strcmp(token->data, "echo") == 0)
	{
		execute_echo(token);
	}
	else if (ft_strcmp(token->data, "env") == 0)
	{
		execute_env(env->data);
	}
	else if (ft_strcmp(token->data, "exit") == 0)
	{
		return (execute_exit(token, exit_status));
	}
	else if (ft_strcmp(token->data, "export") == 0)
	{
		execute_export(token, env);
	}
	else if (ft_strcmp(token->data, "pwd") == 0)
	{
		execute_pwd();
	}
	else if (ft_strcmp(token->data, "unset") == 0)
	{
		execute_unset(token, env);
	}
	else
	{
		*exit_status = execute_execve(token, env);
	}
	return (1);
}
