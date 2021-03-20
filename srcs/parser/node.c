/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 12:31:53 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 12:32:14 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

t_parser_node	*make_node(t_token *con, t_parser_node *l, t_parser_node *r)
{
	t_parser_node	*ret_p;

	if ((ret_p = malloc(sizeof(t_parser_node))))
	{
		ret_p->content = con;
		ret_p->l_node = l;
		ret_p->r_node = r;
	}
	return (ret_p);
}
