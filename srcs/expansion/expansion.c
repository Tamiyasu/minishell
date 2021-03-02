/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 10:12:39 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 14:49:28 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "libft.h"
/*
struct node{
     int data;　　　　　　　　　　   // この要素が保持している何らかのデータ
     struct node *left_ptr;　　   // 左要素へのポインタ
     struct node *right_ptr;  　 // 右要素へのポインタ
}
*/

void	expansion(/*t_lsttoken *token*/t_parser_node *node, /*t_env *env, */int *exit_status)
{
	// while (token != NULL)
	// {
	// 	if (ft_strcmp(token->data, "$?") == 0)
	// 	{
	// 		free(token->data);
	// 		token->data = ft_strdup(ft_itoa(*exit_status));
	// 	}
	// 	token = token->next;
	// }

    if (node == NULL)
        return;

    expansion(node->l_node, exit_status);
	while (node->content != NULL)
	{
    	printf("expansion[%s]\n", node->content->data);
		node->content = node->content->next;
	}
    expansion(node->r_node, exit_status);
}
