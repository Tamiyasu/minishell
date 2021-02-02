/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 23:15:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/01 19:52:31 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execute.h"
#include "libft.h"

/*
** コマンドを実行する　returnで終了ステータスを返す.
*/
int	msh_execute(t_lsttoken *token, t_env *env)
{
	int	data_len;

	data_len = ft_strlen(token->data);

	if (ft_strncmp(token->data, "cd", (data_len + 1)) == 0)
	{
		execute_cd(token, env->data);
	}
	else if (ft_strncmp(token->data, "pwd", (data_len + 1)) == 0)
	{
		execute_pwd();
	}
	return (1);
}

