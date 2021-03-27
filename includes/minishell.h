/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:01:07 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 17:06:34 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define FD_MAX 255
# define EXIT_COMMAND_NOT_EXECUTED 126
# define EXIT_COMMAND_NOT_FOUND 127
# define EXIT_OUT_OF_RANGE_STATUS 255
# define EXIT_SYNTAX_ERROR 258

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <fcntl.h>
# include <termios.h>
# include "libft.h"

extern int		g_exit_status;

typedef struct	s_env
{
	char		**data;
	int			num;
	char		*pwd_data;
	char		*unset_pwd;
	int			oldpwd_flag;
	int			pwd_flag;
	int			shlvl_flag;
}				t_env;

int				faile_func(int result, char **line);
void			free_args(char **args);
char			*error_str(char *str);
void			env_init(t_env *env);
void			env_free(t_env *env);
int				env_search(char **env_data, char *variable_name);
void			env_update_pwddata(t_env *env, char *cd);
void			env_set_data(t_env *env, char **envp);
#endif
