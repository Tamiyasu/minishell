/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/29 22:47:26 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "expansion.h"
#include "execute.h"
#include "terminal.h"
#include "parser.h"
#include "signal_handler.h"
#include "history.h"

int		g_exit_status;

int		return_result(int result, char **line)
{
	if (result == TERM_ERR)
	{
		free(*line);
		exit(1);
	}
	else if (result == TERM_EOF && ft_strlen(*line) == 0)
	{
		free(*line);
		g_exit_status = 0;
		write(1, "exit\n", 5);
		return (-1);
	}
	return (0);
}

int		get_line(char **line)
{
	struct termios	term;
	struct termios	term_save;
	int				result;

	signal(SIGINT, sig_handler_p);
	signal(SIGQUIT, sig_handler_p);
	tcgetattr(0, &term);
	tcgetattr(0, &term_save);
	term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(0, TCSANOW, &term);
	result = terminal(line);
	tcsetattr(0, TCSANOW, &term_save);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (return_result(result, line));
}

void	loop_end_func(char **line, t_parser_node **node)
{
	free_tree(node);
	free(*line);
	*line = NULL;
}

//del
void	print_token(t_token *token)
{
	while (token)
	{
		printf("toke->data[%s][%d]\n", token->data, token->flag);
		token = token->next;
	}
}
//del

void	minishell_loop(t_env *env)
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
		result = lexer(line, &token_list);
		if (!faile_func(result, &line) || token_list == NULL)
			continue ;
		printf("\n--------lexer------------------------\n\n");//del
		print_token(token_list);//del
		result = parser(token_list, &node);
		if (!faile_func(result, &line))
			continue ;
		//printf("\n--------expansion--------------------\n\n");//del
		expansion(node, env);
		printf("--------after expansion--------------\n");//del
		print_token(node->content);//del
		printf("--------check print end--------------\n");//del
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
		return (EXIT_FAILURE);
	minishell_loop(&env);
	env_free(&env);
	history(NULL, 0);
	return (g_exit_status);
}
