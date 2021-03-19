/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:01:07 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 17:00:52 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define FD_MAX 255
# define EXIT_COMMAND_NOT_EXECUTED 126
# define EXIT_COMMAND_NOT_FOUND 127
# define EXIT_OUT_OF_RANGE_STATUS 255

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
# include "libft.h"

extern int	g_exit_status;

typedef struct	s_env
{
	char		**data;
	int			num;
	char		*pwd_data; //PWD存在しようがしないが、cdされるごとに常にcwdirに更新される。unset PWDされた際は、空文字が入る
	char		*unset_pwd; //unset PWD が実行されたcwdirを保存する
	int			oldpwd_flag; //存在しない時は-1となる.
	int			pwd_flag; //存在しない時は-1となる.
}				t_env;

void		free_args(char **args);

/*
** enviroment.c
*/
void	env_init(t_env *env);
void	env_make_data(t_env *env, char **envp);
int		env_search(char **env_data, char *variable_name);
void	env_update_pwddata(t_env *env);
void	env_free(t_env *env);

#endif