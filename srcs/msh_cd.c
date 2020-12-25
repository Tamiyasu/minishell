/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 20:34:40 by ysaito            #+#    #+#             */
/*   Updated: 2020/12/25 19:41:08 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	msh_cd(char **args)
{
	printf("in msh_cd[%s]\n", args[0]);//del
	if (args[1] == NULL)//cdコマンド [引数なし] の時
	{
		ft_putendl_fd("minishell: expected argument to \"cd\"", 1);
	}
	else
	{
		if (chdir(args[1]) == -1)
		{
			strerror(errno);
		}
	}
	return (1);
}
