/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 17:29:51 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 23:13:54 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	free_lst(t_token **token)
{
	t_token *temp;
	t_token *temp_next;

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

t_token	*token_list_last(t_token *token_list)
{
	t_token	*return_p;

	return_p = token_list;
	while (return_p && return_p->next)
	{
		return_p = return_p->next;
	}
	return (return_p);
}

t_token	*token_list_new(void *content)
{
	t_token	*return_p;

	return_p = malloc(sizeof(t_token));
	if (return_p)
	{
		return_p->next = 0;
		return_p->data = content;
		return_p->flag = 0;
	}
	else
		ft_enomem();
	return (return_p);
}

void	token_list_addback(t_token **token_list, t_token *new)
{
	if (*token_list)
		token_list_last(*token_list)->next = new;
	else
		*token_list = new;
}

int		token_lstsize(t_token *token)
{
	int i;

	i = 0;
	while (token)
	{
		i++;
		token = token->next;
	}
	return (i);
}
