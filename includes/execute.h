/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:40:12 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/06 16:12:55 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"
# include "parser.h"

enum pipe_in_out
{
	P_READ,
	P_WRITE
};

void	execute(t_parser_node *node, t_env *env, int *exit_status);
int		execute_command(t_lsttoken *token, t_env *env, int *exit_status);
int		execute_cd(t_lsttoken *token, t_env *env);
int		execute_echo(t_lsttoken *token);
void	execute_execve(t_lsttoken *token, t_env *env);
void	execute_exit(t_lsttoken *token, int *exit_status);
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

#endif