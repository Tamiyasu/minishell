/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 00:56:58 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 13:26:10 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"

int				check_token_type(t_token *token, int last_type)
{
	int	type_i;
	int	token_length;

	token_length = ft_strlen(token->data);
	if (token->data[token_length - 1] == '>')
	{
		if (token_length == 1 || token->data[token_length - 2] != '>')
			type_i = FT_REDIRECT_O_F;
		else
			type_i = FT_REDIRECT_A_F;
	}
	else if (token->data[token_length - 1] == '|')
		type_i = FT_PIPE_F;
	else if (token->data[token_length - 1] == ';')
		type_i = FT_SEMICOLON_F;
	else if (token->data[token_length - 1] == '<')
		type_i = FT_REDIRECT_I_F;
	else if (is_redirect(last_type))
		type_i = FT_FILENAME_F;
	else
		type_i = FT_COMMAND_F;
	return (type_i);
}

int				error_1(t_token *token, t_token *next, t_parser_node *node)
{
	error_str("'");
	error_str(token->data);
	error_str("syntax error near unexpected token `");
	free_lst(&next);
	free_lst(&token);
	free_tree(&node);
	return (0);
}

int				error_2(t_token *token, t_parser_node *node)
{
	error_str("syntax error near unexpected token `newline'");
	free_lst(&token);
	free_tree(&node);
	return (0);
}

void			parser_main(int c_type, t_parser_node **node, t_token *token)
{
	t_parser_node	*c_node;

	if (c_type == FT_COMMAND_F)
		token_list_addback(&(find_c_node(*node))->content, token);
	else if (c_type == FT_SEMICOLON_F)
		*node = make_node(token, *node, NULL);
	else if (c_type == FT_PIPE_F)
	{
		if (*node && (*node)->content->flag == FT_SEMICOLON_F)
			(*node)->r_node = make_node(token, (*node)->r_node, NULL);
		else
			*node = make_node(token, *node, NULL);
	}
	else if (is_redirect(c_type))
	{
		c_node = find_parent_node(*node);
		if (c_node)
			c_node->r_node = make_node(token, c_node->r_node, NULL);
		else
			*node = make_node(token, *node, NULL);
	}
	else if (c_type == FT_FILENAME_F)
		(find_redirect_node(*node))->r_node = make_node(token, NULL, NULL);
}

int				parser(t_token *token, t_parser_node **node_p)
{
	t_parser_node	*node;
	t_token			*next;
	int				last_type;
	int				c_type;

	last_type = FT_EMPTY_F;
	node = make_node(NULL, NULL, NULL);
	while (token)
	{
		next = token->next;
		token->next = NULL;
		c_type = check_token_type(token, last_type);
		if (!check_input(c_type, last_type))
			return (error_1(token, next, node));
		token->flag = c_type;
		parser_main(c_type, &node, token);
		token = next;
		last_type = c_type;
	}
	if (!check_last_input(c_type))
		return (error_2(token, node));
	*node_p = node;
	return (1);
}
