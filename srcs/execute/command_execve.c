/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 14:44:34 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 03:05:43 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "lexer.h"
#include "libft.h"

static char	**execve_format_args(t_token *token, char *command)
{
	char	**args;
	int		lst_num;
	int		idx;

	lst_num = token_lstsize(token);
	args = malloc(sizeof(char *) * (lst_num + 1));
	if (args == NULL)
		ft_enomem();
	idx = 0;
	args[idx] = ft_strdup(command);
	idx++;
	token = token->next;
	while (token != NULL)
	{
		args[idx] = ft_strdup(token->data);
		token = token->next;
		idx++;
	}
	args[idx] = NULL;
	return (args);
}

int	check_path_directory(char *command)
{
	struct stat		stat_buf;

	lstat(command, &stat_buf);
	if ((stat_buf.st_mode & S_IFMT) == 0040000)
	{
		return (1);
	}
	return (0);
}

int	check_permission_exec(char *command)
{
	struct stat		stat_buf;

	lstat(command, &stat_buf);
	if ((stat_buf.st_mode & S_IXUSR) == 00100)
	{
		return (1);
	}
	return (0);
}

void			command_execve(t_token *token, t_env *env)
{
	char	**args;
	int		rc;

	if (!(execve_search_cmdpath(token, env)))
	{
		output_error(token->data, "command not found");
		exit(EXIT_COMMAND_NOT_FOUND);
	}
	args = execve_format_args(token, token->data);
	rc = execve(token->data, args, env->data);
	free_args(args);
	if (rc == -1)
	{
		if (errno == ENOENT)
		{
			output_error(token->data, strerror(errno));
			exit(EXIT_COMMAND_NOT_FOUND);
		}
		if (check_path_directory(token->data))
		{
			output_error(token->data, "is a directory");
			exit(EXIT_COMMAND_NOT_EXECUTED);
		}
		if (errno == ENOEXEC && !(check_permission_exec(token->data)))
			errno = EACCES;
		if (errno == ENOEXEC)
			exit(EXIT_SUCCESS);
		output_error(token->data, strerror(errno));
		exit(EXIT_COMMAND_NOT_EXECUTED);
	}
}
