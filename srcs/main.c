/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 16:56:32 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "expansion.h"
#include "execute.h"
#include "get_next_line.h"
#include "parser.h"
#include "signal_handler.h"
#include "history.h"

int	g_exit_status;

int		faile_func(int result, char **line)
{
	free(*line);
	*line = NULL;
	if (!result)
	{
		g_exit_status = EXIT_SYNTAX_ERROR;
		ft_putendl_fd(error_str("minishell: "), STDERR_FILENO);
		error_str(NULL);
	}
	return (result);
}

int		get_line(char **line)
{
	int result;
	signal(SIGINT, sig_handler_p);
	signal(SIGQUIT, sig_handler_p);

   	struct termios term;
    struct termios term_save;
	tcgetattr(0, &term);
	//term_save = term;
	tcgetattr(0, &term_save);
	term.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(0,TCSANOW, &term);
	result = get_next_line(line);
	tcsetattr(0,TCSANOW, &term_save);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (result == GNL_ERR)
	{
		free(*line);
		exit(1);
	}
	else if (result == GNL_EOF && ft_strlen(*line) == 0)
	{
		free(*line);
		g_exit_status = 0;
		write(1, "exit\n", 5);
		return (-1);
	}
	return (0);
}

void	loop_end_func(char **line, t_parser_node **node)
{
	free_tree(node);
	free(*line);
	*line = NULL;
}

void	msh_loop(t_env *env)
{
	char			*line;
	t_token			*token_list;
	t_parser_node	*node;
	t_info_fd		*msh_fd;
	int				result;

	while (1)
	{
		ft_putstr_fd("minishell>> ", 1);
		if (get_line(&line) == -1)
			break ;
		//printf("\nline=[%s]\n", line);//del
		result = lexer(line, &token_list);
		if (!faile_func(result, &line) || token_list == NULL)
			continue ;
		result = parser(token_list, &node);
		if (!faile_func(result, &line))
			continue ;
		expansion(node, env);
		msh_fd = NULL;
		execute(node, env, msh_fd);
		loop_end_func(&line, &node);
	}
}

int		main(int argc, char *argv[], char *envp[])
{
	t_env	env;

	argc -= argc;
	argv -= (long)argv;
	g_exit_status = 0;
	env_init(&env);
	env_set_data(&env, envp);
	if (env.data == NULL)
	{
		return (EXIT_FAILURE);
	}
	msh_loop(&env);
	env_free(&env);
	printf("-------------------------------\n");
	history(NULL, 0);
	return (g_exit_status);
}
