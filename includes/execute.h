/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/31 20:40:12 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 21:44:42 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"
# include "parser.h"
# define READ 0
# define WRITE 1

typedef struct	s_info_fd
{
	int					fd_num;
	int					fd_save;
	int					flag;
	struct s_info_fd	*next;
}				t_info_fd;

typedef struct	s_sort
{
	int	count;
	int left;
	int	right;
	int	loop_count;
}				t_sort;

void			execute(t_parser_node *node, t_env *env, t_info_fd *fd);
void			exec_redirect(t_parser_node *node,
							t_info_fd *msh_fd, t_env *env,
							void (*func)(t_parser_node *node,
							t_env *env, t_info_fd *msh_fd));
t_info_fd		*redirect_save_fd(t_info_fd *msh_fd, int fd_num, int flag);
int				redirect_check_reserve(t_info_fd *msh_fd, int fd_num,
									int redirect_flag);
t_info_fd		*redirect_save_fd(t_info_fd *msh_fd, int fd_num, int flag);
int				exec_check_builtin(char *token_data);
char			**exec_split_env(t_env *env);
int				command_cd(t_token *token, t_env *env);
int				command_echo(t_token *token);
void			command_execve(t_token *token, t_env *env);
void			command_exit(t_token *token);
int				command_export(t_token *token, t_env *env);
int				command_env(char **env_data);
int				command_pwd(t_env *env);
int				command_unset(t_token *token, t_env *env);
void			output_error(char *err_command, char *err_string);
void			output_no_filename(void);
void			output_error_exit_args(char	*exit_args);
void			unset_error(t_token *token, char *command,
						char *err, int *status);
char			*cwd_wrapper(t_env *env, char *cd);
char			*get_aim_dir(t_env *env, char *cd_str);
void			normalize(char **aim_dir);
t_info_fd		*fd_list_new(int fd_num, int fd_save, int flag);
t_info_fd		*fd_list_last(t_info_fd *msh_fd);
void			fd_list_addback(t_info_fd **msh_fd, t_info_fd *new);
void			free_fd(t_info_fd **msh_fd);
void			reset_fd(t_info_fd *msh_fd);
char			**export_format_variable(t_token *token, t_env *env);
void			export_check_dupl(t_token *token, char **split_tokend);
void			export_cmp_args(t_token *token, char **split_tokend,
							t_env *env, char **split_env);
void			export_sort(char **envp, int *idx, int num);
char			**export_shape_putenv(char *env);
int				export_putenv(t_env *env);
void			unset_save_pwd(char *token_data, t_env *env);
int				execve_search_cmdpath(t_token *token, t_env *env);
void			exec_pipe(t_parser_node *node, t_env *env, t_info_fd *msh_fd);
void			exec_command(t_token *token, t_env *env, int child_flag);
void			set_signals(void (*fc)(int));
int				get_exit_status(int pid_status);
void			command_builtin(t_token *token, t_env *env);
int				exec_is_builtin(char *token_data);
#endif
