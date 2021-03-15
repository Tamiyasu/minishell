/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/16 00:58:20 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "expansion.h"
#include "execute.h"
#include "libft.h"
#include "get_next_line.h"
#include "parser.h"
#include "signal_handler.h"

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

t_lsttoken *find_first_commnd_node(t_parser_node *node)
{
	printf("node : %p\n", node);
	printf("content : %p\n", node->content);
	printf("flag : %d\n", node->content->flag);
	while (node->content->flag != FT_COMMAND_F)
		node = node->l_node;
	return (node->content);
}


void	msh_loop(t_env *env, int *exit_status)
{
	char			*line;
	t_lsttoken		*token_list;
	t_parser_node	*node;
	t_info_fd		*fd;

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
		} else if (gnl_result == GNL_EOF && ft_strlen(line) == 0){
			*exit_status = 0;
			write(1, "exit\n", 5);
			break;
		}
		// printf("gnl_result : %d\n", gnl_result);
		token_list = lexer(line);
		if (token_list == NULL)
		{
			free(line);
			continue ;
		}
		// print_token(token_list, "check token");

		node = parser(token_list);
		// node_print(node, 0);
		// printf("----------------------------end node_print\n\n");

		expansion(node, env, exit_status);
		// print_token(token_list, "check token");
		// printf("----------------------------end expansion_print\n\n");


		//init_fd(&fd);
		fd = NULL;
		execute(node, env, exit_status, fd); //exitコマンド実行時にreturn(0)がくる
		reset_fd(fd);
		free_fd(&fd);

		free_tree(&node);
		free(line);
		line = NULL;
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_env	env;
	int		exit_status;

	argc -= argc;//del
	argv -= (long)argv;//del
	exit_status = 0;//de;
	msh_env_init(&env);
	msh_env_make_data(&env, envp);
	if (env.data == NULL)
	{
		return (EXIT_FAILURE);
	}
	msh_loop(&env, &exit_status);
	msh_env_free(&env);
	return (exit_status);
}
