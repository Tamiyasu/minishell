/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 17:11:47 by ysaito            #+#    #+#             */
/*   Updated: 2020/12/25 17:57:47 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	msh_env(char **envp)
{
	printf("in msh_env\n");//del
	int	idx;

	idx = 0;
	while (envp[idx] != NULL)
	{
		ft_putendl_fd(envp[idx], 1);
		idx++;
	}
	return (1);
}
