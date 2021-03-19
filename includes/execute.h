/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:40:12 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 14:41:15 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# define FD_MAX	255

# define EXIT_COMMAND_NOT_EXECUTED 126
# define EXIT_COMMAND_NOT_FOUND 127
# define EXIT_OUT_OF_RANGE_STATUS 255

# include "minishell.h"
# include "parser.h"

enum pipe_in_out
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
int		execute_cd(t_lsttoken *token, t_env *env);
int		execute_echo(t_lsttoken *token);
void	command_execve(t_lsttoken *token, t_env *env);
void	execute_exit(t_lsttoken *token);
int		execute_export(t_lsttoken *token, t_env *env);
int		execute_env(char **env_data);
int		execute_pwd(void);
int		execute_unset(t_lsttoken *token, t_env *env);
void	init_fd(t_info_fd *fd);
void	reset_fd(t_info_fd *fd);
void	free_fd(t_info_fd **fd);
void	output_error(char *err_command, char *err_string);
void	output_no_filename(void);
void	output_error_exit_args(char	*exit_args);

/*
** unset, export
*/
char	**execute_split_env(t_env *env);

/*
** export
*/
int		export_putenv(t_env *env);

#endif