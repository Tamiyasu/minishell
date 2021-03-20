/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:40:12 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 22:42:44 by ysaito           ###   ########.fr       */
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
	int					fd_num;
	int					fd_save;
	int					flag;
	struct s_info_fd	*next;
}				t_info_fd;

void	execute(t_parser_node *node, t_env *env, t_info_fd *fd);
void	exec_redirect(t_parser_node *node, t_info_fd *msh_fd, t_env *env,
					void (*func)(t_parser_node *node,
					t_env *env, t_info_fd *msh_fd));
t_info_fd	*redirect_save_fd(t_info_fd *msh_fd, int fd_num, int flag);
int	redirect_check_reserve(t_info_fd *msh_fd, int fd_num, int redirect_flag);
t_info_fd	*redirect_save_fd(t_info_fd *msh_fd, int fd_num, int flag);
int		exec_check_builtin(char *token_data);
int		command_cd(t_token *token, t_env *env);
int		command_echo(t_token *token);
void	command_execve(t_token *token, t_env *env);
void	command_exit(t_token *token);
int		command_export(t_token *token, t_env *env);
int		command_env(char **env_data);
int		command_pwd(t_env *env);
int		command_unset(t_token *token, t_env *env);
void	output_error(char *err_command, char *err_string);
void	output_no_filename(void);
void	output_error_exit_args(char	*exit_args);
void	unset_error(t_token *token, int *status);
char	*cwd_wrapper(t_env *env, char *cd);

/*
** exec_fd.c
*/
t_info_fd	*fd_list_new(int fd_num, int fd_save, int flag);
t_info_fd	*fd_list_last(t_info_fd *msh_fd);
void		fd_list_addback(t_info_fd **msh_fd, t_info_fd *new);
void		free_fd(t_info_fd **msh_fd);
void		reset_fd(t_info_fd *msh_fd);

/*
** unset, export
*/
char		**exec_split_env(t_env *env);

/*
** export
*/
int			export_putenv(t_env *env);

#endif