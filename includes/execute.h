/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:40:12 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/14 00:36:19 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"
# include "parser.h"

enum pipe_in_out
{
	READ,
	WRITE
};

typedef struct	s_info_fd
{
	//int			open_fd;
	int			save_stdin;
	int			save_stdout;
	int			save_stderr;
	int			redirect_i;
	int			redirect_o;
	int			redirect_err;
	int			fd_num;
}				t_info_fd;


void	execute(t_parser_node *node, t_env *env, int *exit_status, t_info_fd *fd);
int		exec_check_builtin(char *token_data);
int		execute_cd(t_lsttoken *token, t_env *env);
int		execute_echo(t_lsttoken *token);
void	command_execve(t_lsttoken *token, t_env *env);
void	execute_exit(t_lsttoken *token, int *exit_status);
int		execute_export(t_lsttoken *token, t_env *env);
int		execute_env(char **env_data);
int		execute_pwd(void);
int		execute_unset(t_lsttoken *token, t_env *env);
void		init_fd(t_info_fd *fd);

/*
** unset, export
*/
char	**execute_split_env(t_env *env);

/*
** export
*/
int		export_putenv(t_env *env);

#endif