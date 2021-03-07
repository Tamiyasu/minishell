/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:01:07 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/07 17:27:55 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <dirent.h>
# include <fcntl.h>

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
** msh_env.c
*/
void	msh_env_init(t_env *env);
void	msh_env_make_data(t_env *env, char **envp);
int		msh_env_search(char **env_data, char *variable_name);
void	msh_env_update_pwddata(t_env *env);
void	msh_env_free(t_env *env);

#endif