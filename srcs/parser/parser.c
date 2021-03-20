/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 00:56:58 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 11:01:30 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include "libft.h"

t_token			*lexer_lstadd_back(t_token **token, t_token *new)
{
	t_token *token_i;

	if (!(*token))
	{
		*token = new;
		return (new);
	}
	token_i = *token;
	while (token_i->next)
		token_i = token_i->next;
	token_i->next = new;
	return (*token);
}

t_parser_node	*free_tree(t_parser_node **node)
{
	if ((*node))
	{
		free_lst(&((*node)->content));
		free_tree(&((*node)->l_node));
		free_tree(&((*node)->r_node));
	}
	free(*node);
	*node = NULL;
	return (*node);
}

int				is_redirect(int flag)
{
	return (
		flag == FT_REDIRECT_O_F ||
		flag == FT_REDIRECT_A_F ||
		flag == FT_REDIRECT_I_F);
}

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
t_parser_node	*find_c_node(t_parser_node *node)
{
	t_parser_node	*ret_node;

	ret_node = NULL;
	if (node->content == NULL || node->content->flag == FT_COMMAND_F)
	{
		ret_node = node;
	}
	else if (node->content->flag == FT_PIPE_F 
			|| node->content->flag == FT_SEMICOLON_F)
	{
		if (node->r_node)
		{
			ret_node = find_c_node(node->r_node);
			if (!ret_node)
			{
				node->r_node = malloc(sizeof(t_parser_node));
				node->r_node->content = NULL;
				node->r_node->r_node = NULL;
				node->r_node->l_node = NULL;
				ret_node = node->r_node;
			}
		}
		else
		{
			node->r_node = malloc(sizeof(t_parser_node));
			node->r_node->content = NULL;
			node->r_node->r_node = NULL;
			node->r_node->l_node = NULL;
			ret_node = node->r_node;
		}
	}
	else if (is_redirect(node->content->flag))
	{
		while (node && node->content && is_redirect(node->content->flag))
		{
			if (!node->l_node)
			{
				node->l_node = malloc(sizeof(t_parser_node));
				node->l_node->content = NULL;
				node->l_node->r_node = NULL;
				node->l_node->l_node = NULL;
			}
			node = node->l_node;
		}
		ret_node = node;
	}
	return (ret_node);
}

t_parser_node	*find_parent_node(t_parser_node   *node)
{
	t_parser_node	*ret_node;

	ret_node = NULL;
	if (node && node->content && (node->content->flag == FT_PIPE_F || node->content->flag == FT_SEMICOLON_F))
	{
		if (node->r_node)
		{
			ret_node = find_parent_node(node->r_node);
		}
		if (!ret_node)
		{
			ret_node = node;
		}
	}
	return (ret_node);
}

t_parser_node	*find_redirect_node(t_parser_node   *node)
{
	t_parser_node	*ret_node;

	ret_node = NULL;
	if (node && is_redirect(node->content->flag))
	{
		ret_node = node;
	}
	else if (node && (node->content->flag == FT_PIPE_F || node->content->flag == FT_SEMICOLON_F))
	{
		if (node->r_node)
		{
			ret_node = find_redirect_node(node->r_node);
		}
	}
	return (ret_node);
}

int check_input(int c_type, int last_type)
{
	if (last_type == FT_EMPTY_F ||
		last_type == FT_PIPE_F ||
		last_type == FT_SEMICOLON_F ||
		is_redirect(last_type)
	)
	{
		if (c_type == FT_PIPE_F)
			return (0);
		if (c_type == FT_SEMICOLON_F)
			return (0);
	}
	if (is_redirect(last_type))
	{
		if (is_redirect(c_type))
		{
			return (0);
		}
	}
	return (1);
}

int				check_last_input(int c_type)
{
	if (c_type == FT_PIPE_F)
		return (0);
	if (is_redirect(c_type))
		return (0);
	return (1);
}

t_parser_node	*make_node(t_token *con, t_parser_node *l, t_parser_node *r)
{
	t_parser_node	*ret_p;

	if((ret_p = malloc(sizeof(t_parser_node))))
	{
		ret_p->content = con;
		ret_p->l_node = l;
		ret_p->r_node = r;
	}
	return (ret_p);
}

int				parser(t_token *token_list, t_parser_node **node_p)
{
	t_parser_node	*node;
	t_token			*token;
	t_token			*next;
	t_parser_node	*c_node;
	int				last_type;
	int				c_type;

	last_type = FT_EMPTY_F;
	c_type = FT_EMPTY_F;
	token = token_list;
	node = make_node(NULL, NULL, NULL);
	while (token)
	{
		next = token->next;
		token->next = NULL;
		c_type = check_token_type(token, last_type);
		if (!check_input(c_type, last_type))
		{
			error_str("'");
			error_str(token->data);
			error_str("syntax error near unexpected token `");
			free_lst(&next);
			free_lst(&token);
			free_tree(&node);
			return (0);
		}
		token->flag = c_type;
		if (c_type == FT_COMMAND_F)
		{
			c_node = find_c_node(node);
			c_node->content = lexer_lstadd_back(&c_node->content, token);
		}
		else if (c_type == FT_SEMICOLON_F)
			node = make_node(token, node, NULL);
		else if (c_type == FT_PIPE_F)
		{
			if (node && node->content->flag == FT_SEMICOLON_F)
				c_node->r_node = make_node(token, node->r_node, NULL);
			else
				node = make_node(token, node, NULL);
		}
		else if (is_redirect(c_type))
		{
			c_node = find_parent_node(node);
			if (c_node)
				c_node->r_node = make_node(token, c_node->r_node, NULL);
			else
				node = make_node(token, node, NULL);
		}
		else if (c_type == FT_FILENAME_F)
		{
			c_node = find_redirect_node(node);
			c_node->r_node = make_node(token, NULL, NULL);
		}
		token = next;
		last_type = c_type;
	}
	if (!check_last_input(c_type))
	{
		error_str("syntax error near unexpected token `newline'");
		free_lst(&token);
		free_tree(&node);
		return (0);
	}
	*node_p = node;
	return (1);
}
