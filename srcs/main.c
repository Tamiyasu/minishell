/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 23:04:22 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "expansion.h"
#include "execute.h"
#include "get_next_line.h"
#include "parser.h"
#include "signal_handler.h"

int	g_exit_status;

void	free_args(char **args)
{
	int	idx;

	idx = 0;
	if (args == NULL)
	{
		return ;
	}
	while (args[idx] != NULL)
	{
		free(args[idx]);
		idx++;
	}
	free(args);
	args = NULL;
}

t_token	*find_first_commnd_node(t_parser_node *node)
{
	printf("node : %p\n", node);
	printf("content : %p\n", node->content);
	printf("flag : %d\n", node->content->flag);
	while (node->content->flag != FT_COMMAND_F)
		node = node->l_node;
	return (node->content);
}

char	*error_str(char *str)
{
	static char	*s_str;
	char		*tmp;

	if (str)
	{
		if (s_str)
		{
			tmp = s_str;
			s_str = ft_strjoin(str, s_str);
			free(tmp);
		}
		else
			s_str = ft_strdup(str);
	}
	else
	{
		free(s_str);
		s_str = NULL;
	}
	return (s_str);
}

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
	result = get_next_line(line);
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
		result = lexer(line, &token_list);
		if (token_list == NULL)
		{
			faile_func(result, &line);
			continue ;
		}
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
	return (g_exit_status);
}
