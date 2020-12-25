/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 16:46:48 by ysaito            #+#    #+#             */
/*   Updated: 2020/12/24 17:33:56 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	msh_exit(char **args)
{
	printf("in msh_exit[%s]\n", args[0]);//del
	// if (args != NULL)
	// {
	// 	free_args(args);
	// }
	return (0);
}
