/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 22:15:30 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 16:51:02 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

char *cwd_wrapper()
{
	char *ret_str;

	ret_str = getcwd(NULL, 0);
	if(ret_str == NULL)
	{
		error_str("No such file or directory");
		error_str("getcwd: cannot access parent directories: ");
	}
	return (ret_str);
}

int	execute_pwd(void)
{
	char	*cwdir;

	cwdir = cwd_wrapper();
	if (cwdir == NULL)
	{
		error_str("error retrieving current directory: ");
		ft_putendl_fd(error_str("pwd: "), 2);
		error_str(NULL);
		//ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(cwdir, STDOUT_FILENO);
	free(cwdir);
	return (EXIT_SUCCESS);
}
