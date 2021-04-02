/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 14:31:55 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 00:20:21 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

void	set_cwd_str(t_env *env, char **cwd_str, char *tmp)
{
	if (env->pwd_data)
		*cwd_str = ft_strdup(env->pwd_data);
	else if (tmp == NULL)
	{
		error_str("No such file or directory");
		error_str("getcwd: cannot access parent directories: ");
		*cwd_str = ft_strdup("");
	}
}
/*
void	set_cwd_str2(char **cwd_str, char *cd)
{
	char	*tmp;

	if (ft_strlen(*cwd_str) > 0
		&& (*cwd_str)[ft_strlen(*cwd_str) - 1] != '/')
	{
		tmp = *cwd_str;
		*cwd_str = ft_strjoin(tmp, "/");
		free(tmp);
	}
	tmp = *cwd_str;
	*cwd_str = ft_strjoin(tmp, cd);
	free(tmp);
}
*/
//shell-init: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory

char	*cwd_wrapper(char *cd)
{
	static char	*cwd_str;
	printf("cwd_str:[%s]\n", cwd_str);
	if (cd)
	{
		free(cwd_str);
		cwd_str = ft_strdup(cd);
	}
	if (cwd_str == NULL)
	{
		cwd_str = getcwd(NULL, 0);
		printf("works!\n");
	}
	if (cwd_str == NULL)
	{
		printf("works2!\n");
		error_str("shell-init: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory");
	}
	printf("cwd_str:[%s]\n", cwd_str);
	return (cwd_str);
}

int		command_pwd(void)
{
	char	*cwdir;

	cwdir = cwd_wrapper(NULL);
	if (cwdir == NULL)
	{
		error_str("error retrieving current directory: ");
		ft_putendl_fd(error_str("pwd: "), STDERR_FILENO);
		error_str(NULL);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(cwdir, STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
