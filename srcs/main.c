/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 23:38:47 by ysaito           ###   ########.fr       */
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

t_token *find_first_commnd_node(t_parser_node *node)
{
	printf("node : %p\n", node);
	printf("content : %p\n", node->content);
	printf("flag : %d\n", node->content->flag);
	while (node->content->flag != FT_COMMAND_F)
		node = node->l_node;
	return (node->content);
}

char *error_str(char *str)
{
	static char *s_str;
	char *tmp;

	if (str)
	{
		if(s_str)
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
	return s_str;
}

void	msh_loop(t_env *env)
{
	char			*line;
	t_token		*token_list;
	t_parser_node	*node;
	t_info_fd		*msh_fd;
	int				result;

	line = NULL;
	token_list =  NULL;
	while (1)
	{
		ft_putstr_fd("minishell>> ", 1);
		signal(SIGINT, sig_handler_p);
		signal(SIGQUIT, sig_handler_p);
		int gnl_result = get_next_line(&line);
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (gnl_result == GNL_ERR)
		{
			// printf("here !\n");
		}
		else if (gnl_result == GNL_EOF && ft_strlen(line) == 0)
		{
			g_exit_status = 0;
			write(1, "exit\n", 5);
			break;
		}
		// printf("gnl_result : %d\n", gnl_result);
		result = lexer(line, &token_list);
		if (!result)
		{
			free(line);
			ft_putendl_fd(error_str("minishell: "), STDERR_FILENO);
			error_str(NULL);
			continue;
		}
		if (token_list == NULL)//スペースかタブのみが入力された時
		{
			free(line);
			continue ;
		}
		//print_token(token_list, "check token");

		result = parser(token_list, &node);
		if(!result)
		{
			free(line);
			ft_putendl_fd(error_str("minishell: "), STDERR_FILENO);
			error_str(NULL);
			continue;
		}
		// node_print(node, 0);
		// printf("----------------------------end node_print\n\n");

		expansion(node, env);
		// print_token(token_list, "check token");
		// printf("----------------------------end expansion_print\n\n");

		msh_fd = NULL;
		execute(node, env, msh_fd); //exitコマンド実行時にreturn(0)がくる
		free_tree(&node);
		free(line);
		line = NULL;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_env	env;

	argc -= argc;//del
	argv -= (long)argv;//del
	g_exit_status = 0;
	env_init(&env);
	env_make_data(&env, envp);
	if (env.data == NULL)
	{
		return (EXIT_FAILURE);
	}
	msh_loop(&env);
	env_free(&env);
	return (g_exit_status);
}
