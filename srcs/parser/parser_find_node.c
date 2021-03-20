/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_find_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 12:51:40 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 13:04:14 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_parser_node	*find_c_node_p_and_sc(t_parser_node *node)
{
	t_parser_node	*ret_node;

	if (node->r_node)
	{
		ret_node = find_c_node(node->r_node);
		if (!ret_node)
			node->r_node = make_node(NULL, NULL, NULL);
		if (!ret_node)
			ret_node = node->r_node;
	}
	else
	{
		node->r_node = make_node(NULL, NULL, NULL);
		ret_node = node->r_node;
	}
	return (ret_node);
}

t_parser_node	*find_c_node(t_parser_node *node)
{
	t_parser_node	*ret_node;

	ret_node = NULL;
	if (node->content == NULL || node->content->flag == FT_COMMAND_F)
		ret_node = node;
	else if (node->content->flag == FT_PIPE_F
			|| node->content->flag == FT_SEMICOLON_F)
		ret_node = find_c_node_p_and_sc(node);
	else if (is_redirect(node->content->flag))
	{
		while (node && node->content && is_redirect(node->content->flag))
		{
			if(!node->l_node)
				node->l_node = make_node(NULL, NULL, NULL);
			node = node->l_node;
		}
		ret_node = node;
	}
	return (ret_node);
}

t_parser_node	*find_parent_node(t_parser_node *node)
{
	t_parser_node	*ret_node;

	ret_node = NULL;
	if (node && node->content
		&& (node->content->flag == FT_PIPE_F
			|| node->content->flag == FT_SEMICOLON_F))
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

t_parser_node	*find_redirect_node(t_parser_node *node)
{
	t_parser_node	*ret_node;

	ret_node = NULL;
	if (node && is_redirect(node->content->flag))
	{
		ret_node = node;
	}
	else if (node && (node->content->flag == FT_PIPE_F
		|| node->content->flag == FT_SEMICOLON_F))
	{
		if (node->r_node)
		{
			ret_node = find_redirect_node(node->r_node);
		}
	}
	return (ret_node);
}
