/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 22:15:30 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 16:32:04 by tmurakam         ###   ########.fr       */
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
		ft_putendl_fd("pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory", 2);
		//ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(cwdir, STDOUT_FILENO);
	free(cwdir);
	return (EXIT_SUCCESS);
}
