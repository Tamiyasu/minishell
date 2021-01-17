/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 22:15:30 by ysaito            #+#    #+#             */
/*   Updated: 2020/12/24 17:33:48 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	msh_pwd(char **args)
{
	char	*cwdir;

	//unused parameter 'args'の応急処置
	printf("in msh_pwd[%s]\n", args[0]);//del
	// if (args != NULL)
	// {
	// 	free_args(args);
	// }

	cwdir = getcwd(NULL, 0);//このように引数を指定してあげると、getcwdの方で、buf[1024]で取ってくれる。
	if (cwdir == NULL)
	{
		strerror(errno);
		return (1);
	}
	ft_putendl_fd(cwdir, 1);
	free(cwdir);
	return (1);
}
