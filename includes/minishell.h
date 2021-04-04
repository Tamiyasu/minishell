/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:01:07 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/04 10:51:53 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define FD_MAX 255
# define SHLVL_MAX 999
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
	char		*oldpwd_data;
	char		*pwd_data;
	char		*unset_pwd;
	int			oldpwd_flag;
	int			pwd_flag;
	int			shlvl_flag;
}				t_env;

typedef struct	s_data
{
	int			length;
	int			start;
	int			idx;
}				t_data;

void			data_increment(t_data *data);
t_data			*data_init(void);
int				faile_func(int result, char **line);
void			free_args(char **args);
char			*error_str(char *str);
void			env_init(t_env *env);
void			env_free(t_env *env);
int				env_search(char **env_data, char *variable_name);
void			env_update_pwddata(t_env *env, char *cd);
void			env_set_data(t_env *env, char **envp);
void			error_shlvl(int shlvl_num);
void			free_set(char **s1, char *s2);
int				arr_size(char **null_ended_strs);
char			*strs_join(char **strs, char *enc, char *f);
void			join_free(char **s1, char *s2);
#endif
