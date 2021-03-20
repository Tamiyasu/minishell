/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:40:12 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 23:20:34 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"
# include "parser.h"

enum			e_pipe_in_out
{
	READ,
	WRITE
};

typedef struct	s_info_fd
{
	int					fd_num;
	int					fd_save;
	int					flag;
	struct s_info_fd	*next;
}				t_info_fd;

void	execute(t_parser_node *node, t_env *env, t_info_fd *fd);
int		exec_check_builtin(char *token_data);
int		execute_cd(t_token *token, t_env *env);
int		execute_echo(t_token *token);
void	command_execve(t_token *token, t_env *env);
void	execute_exit(t_token *token);
int		execute_export(t_token *token, t_env *env);
int		execute_env(char **env_data);
int		execute_pwd(t_env *env);
int		execute_unset(t_token *token, t_env *env);
void	init_fd(t_info_fd *fd);
void	reset_fd(t_info_fd *fd);
void	free_fd(t_info_fd **fd);
void	output_error(char *err_command, char *err_string);
void	output_no_filename(void);
void	output_error_exit_args(char	*exit_args);
char	*cwd_wrapper(t_env *env, char *cd);

/*
** unset, export
*/
char	**execute_split_env(t_env *env);

/*
** export
*/
int		export_putenv(t_env *env);

#endif
