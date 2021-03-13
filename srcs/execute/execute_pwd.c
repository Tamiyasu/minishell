/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 22:15:30 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/13 18:27:50 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

int	execute_pwd(void)
{
	char	*cwdir;

	cwdir = getcwd(NULL, 0);//このように引数を指定してあげると、getcwdの方で、buf[1024]で取ってくれる。
	if (cwdir == NULL)
	{
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
	ft_putendl_fd(cwdir, 1);
	free(cwdir);
	return (0);
}
