/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 17:29:51 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 20:53:24 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "lexer.h"
#include "libft.h"

void	free_lst(t_lsttoken **token)
{
	t_lsttoken *temp;
	t_lsttoken *temp_next;

	temp = *token;
	while (temp != NULL)
	{
		temp_next = temp->next;
		free(temp->data);
		free(temp);
		temp = temp_next;
	}
	*token = NULL;
}

t_lsttoken	*token_list_last(t_lsttoken *token_list)
{
	t_lsttoken	*return_p;

	return_p = token_list;
	while (return_p && return_p->next)
	{
		return_p = return_p->next;
	}
	return (return_p);
}

t_lsttoken	*token_list_new(void *content)
{
	t_lsttoken	*return_p;

	return_p = malloc(sizeof(t_lsttoken));
	if (return_p)
	{
		return_p->next = 0;
		return_p->data = content;
		return_p->flag = 0;
	}
	return (return_p);
}

void	token_list_addback(t_lsttoken **token_list, t_lsttoken *new)
{
	if (*token_list)
		token_list_last(*token_list)->next = new;
	else
		*token_list = new;
}
