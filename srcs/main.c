/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 12:02:26 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/12 21:01:33 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "expansion.h"
#include "execute.h"
#include "libft.h"
#include "get_next_line.h"
#include "parser.h"

#include <unistd.h>

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
	//int		loop_status;
	t_lsttoken		*token_list;
	t_parser_node	*node;
	t_info_fd		fd;

	line = NULL;
	//loop_status = 1;
	token_list =  NULL;
	while (1)
	{
		ft_putstr_fd("minishell>> ", 1);
		int gnl_result = get_next_line(&line);
		if (gnl_result == GNL_ERR)
		{
			printf("here !\n");
		} else if (gnl_result == GNL_EOF && ft_strlen(line) == 0){
			*exit_status = 0;
			write(1, "exit\n", 5);
			break;
		}
		printf("gnl_result : %d\n", gnl_result);
		token_list = lexer(line);
		if (token_list == NULL)
		{
			free(line);
			continue ;
		}
		print_token(token_list, "check token");

		node = parser(token_list);
		node_print(node, 0);
		printf("----------------------------end node_print\n\n");

		expansion(node, env, exit_status);

		init_fd(&fd);
		execute(node, env, exit_status, &fd); //exitコマンド実行時にreturn(0)がくる
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
