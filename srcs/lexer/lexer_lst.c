/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 17:29:51 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 20:47:36 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "lexer.h"
#include "libft.h"

void		lexer_lstinit(t_lsttoken *token, int input_len)
{
	token->data = malloc(sizeof(char *) * (input_len + 1));
	if (token->data == NULL)
		return ;
	ft_bzero(token->data, input_len);
	token->flag = 0;
	token->next = NULL;
}

t_lsttoken	*lexer_lstnew(int input_len)
{
	t_lsttoken	*return_p;

	return_p = malloc(sizeof(t_lsttoken));
	if (return_p == NULL)
		return (NULL);
	lexer_lstinit(return_p, input_len);
	if  (return_p->data == NULL)
	{
		free(return_p);
		return (NULL);
	}
	return (return_p);
}

t_lsttoken	*lexer_lstadd(t_lsttoken *token, int input_len)
{
	token->next = malloc(sizeof(t_lsttoken));
	if (token->next == NULL)
		return (NULL);
	token = token->next;
	lexer_lstinit(token, input_len);
	return (token);
}

t_lsttoken	*lexer_lstlast(t_lsttoken *token)
{
	t_lsttoken *return_p;

	return_p = token;
	while (return_p && return_p->next)
	{
		return_p = return_p->next;
	}
	return (return_p);
}

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
