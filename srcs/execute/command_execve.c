/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execve.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/12 14:44:34 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/16 14:57:32 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "lexer.h"
#include "libft.h"

static int	execve_count_lst(t_lsttoken *token)
{
	int	num;

	num = 0;
	while (token != NULL)
	{
		num++;
		token = token->next;
	}
	return (num);
}

static char	**execve_format_args(t_lsttoken *token, char *command)
{
	char	**args;
	int		lst_num;
	int		idx;

	lst_num = execve_count_lst(token);
	args = malloc(sizeof(char *) * (lst_num + 1));
	if (args == NULL)
	{
		return (NULL);
	}
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

int	check_path_directory(char *command/*, int *exit_status*/)
{
	struct stat		stat_buf;

	lstat(command, &stat_buf);
	if ((stat_buf.st_mode & S_IFMT) == 0040000)
	{
		return (1);
	}
	return (0);
}

int	search_command_path(t_lsttoken *token, t_env *env)
{
	char			**path_value;
	int				idx;
	DIR				*dp;
	struct dirent	*dirp;
	char			*tmp;

	if (token->data[0] == '.' || token->data[0] == '/')
		return (1);
	idx = msh_env_search(env->data, "PATH");
	path_value = ft_split(&env->data[idx][5], ':');
	idx= 0;
	while (path_value[idx] != NULL)
	{
		dp = opendir(path_value[idx]);
		if (dp == NULL)
		{
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			return (0);
		}
		while ((dirp = readdir(dp)) != NULL)
		{
			if (ft_strcmp(token->data, dirp->d_name) == 0)
			{
				tmp = ft_strjoin("/", token->data);
				free(token->data);
				token->data = ft_strjoin(path_value[idx],  tmp);
				free(tmp);
				tmp = NULL;
				closedir(dp);
				free_args(path_value);
				return (1);
			}
		}
		closedir(dp);
		idx++;
	}
	free_args(path_value);
	return (0);
}

//1 =エラー
//126 connamd not executable(実行権限なし)
//127 command not found

//ENOENT 2 /* No such file or directory */->127
//ENOEXEC 8      /* Exec format error */
//EACCES 13      /* Permission denied */


void			command_execve(t_lsttoken *token, t_env *env)
{
	//char	*command;
	char	**args;
	int		rc;

	if (!(search_command_path(token, env)))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(token->data, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(127);
	}
	args = execve_format_args(token, token->data);
	rc = execve(token->data, args, env->data);
	if (rc == -1)
	{
		free_args(args);
		//printf("command=[%s]execve errno=[%d][%s]\n", token->data, errno, strerror(errno));
		if (ft_strcmp(token->data, ".") == 0)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putendl_fd(".: filename argument requirred", STDERR_FILENO);
			ft_putendl_fd(".: usage: . filename [arguments]", STDERR_FILENO);
			exit(2);
		}
		if (check_path_directory(token->data))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(token->data, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			exit(126);
		}
		if (errno == EACCES)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(token->data, STDERR_FILENO);
			ft_putstr_fd(": " , STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			exit(126);
		}
		if (errno == 8)//ENOEXEC 8      /* Exec format error */
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(token->data, STDERR_FILENO);
			ft_putstr_fd(": " , STDERR_FILENO);
			ft_putendl_fd(strerror(errno), STDERR_FILENO);
			exit(126);
		}
		// ft_putstr_fd("minishell: ", STDERR_FILENO);
		// ft_putstr_fd(token->data, STDERR_FILENO);
		// ft_putstr_fd(": ", STDERR_FILENO);
		// ft_putendl_fd(strerror(errno), STDERR_FILENO);

		// if (errno == 13) /* Permission denied */
		// 	exit(126);
		exit (errno);
	}
	free_args(args);
}
