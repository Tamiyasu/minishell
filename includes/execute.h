/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:40:12 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/28 19:48:10 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		execute(t_lsttoken *token, t_env *env, int *exit_status);
int		execute_command(t_lsttoken *token, t_env *env, int *exit_status);
int		execute_cd(t_lsttoken *token, t_env *env);
int		execute_echo(t_lsttoken *token);
int		execute_execve(t_lsttoken *token, t_env *env);
int		execute_exit(t_lsttoken *token, int *exit_status);
int		execute_export(t_lsttoken *token, t_env *env);
int		execute_env(char **env_data);
int		execute_pwd(void);
int		execute_unset(t_lsttoken *token, t_env *env);

/*
** unset, export
*/
char	**execute_split_env(t_env *env);

/*
** export
*/
int		export_putenv(t_env *env);