/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 17:29:51 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 16:44:05 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "lexer.h"
#include "libft.h"

void		lexer_lstinit(t_lexer_token *token, int input_len)
{
	token->data = malloc(sizeof(char *) * (input_len + 1));
	if (token->data == NULL)
		return ;
	ft_bzero(token->data, input_len);
	token->flag = 0;
	token->next = NULL;
}

t_lexer_token	*lexer_lstnew(int input_len)
{
	t_lexer_token	*return_p;

	return_p = malloc(sizeof(t_lexer_token));
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

t_lexer_token	*lexer_lstadd(t_lexer_token *token, int input_len)
{
	token->next = malloc(sizeof(t_lexer_token));
	if (token->next == NULL)
		return (NULL);
	token = token->next;
	lexer_lstinit(token, input_len);
	return (token);
}

t_lexer_token	*lexer_lstlast(t_lexer_token *token)
{
	t_lexer_token *return_p;

	return_p = token;
	while (return_p && return_p->next)
	{
		return_p = return_p->next;
	}
	return (return_p);
}

void	free_lst(t_lexer_token **token)
{
	t_lexer_token *temp;
	t_lexer_token *temp_next;

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

// t_lexer_token	*token_list
t_lexer_token	*token_list_last(t_lexer_token *token_list)
{
	t_lexer_token	*return_p;

	return_p = token_list;
	while (return_p && return_p->next)
	{
		return_p = return_p->next;
	}
	return (return_p);
}

t_lexer_token	*token_list_new(void	*content)
{
	t_lexer_token	*return_p;

	return_p = malloc(sizeof(t_lexer_token));
	if (return_p)
	{
		return_p->next = 0;
		return_p->data = content;
		return_p->flag = 0;
	}
	return (return_p);
}

void	token_list_addback(t_lexer_token **token_list, t_lexer_token *new)
{
	if (*token_list)
		token_list_last(*token_list)->next = new;
	else
		*token_list = new;
}


// 	while (input[idx] != '\0')
// 	{
// 		if (lexer->state == STATE_GENERAL)
// 		{
// 			if (input[idx] == '\'' || input[idx] == '\"')
// 				token = lexer_set_quote(token, lexer, input[idx]);
// 			else if (input[idx] == ' ' || input[idx] == '\t')
// 			{
// 				token = lexer_skip_space(token, lexer, input, &idx, input_len);
// 				continue ;
// 			}
// 			else if (input[idx] == '<' || input[idx]  == '>')
// 			{
// 				//token = lexer_set_redirect(token, lexer, input, &idx, input_len);
// 				if  (input[idx] == '<')
// 				{
// 					if (lexer->data_idx != 0)
// 					{
// 						token->data[lexer->data_idx] = '\0';
// 						lexer->data_idx = 0;
// 						token =  lexer_lstadd(token, input_len);
// 					}
// 					token->data[0] = '<';
// 					token->data[1] = '\0';
// 					if (input[idx + 1] != '\0')
// 					{
// 						token = lexer_lstadd(token, input_len);
// 						lexer->data_idx = 0;
// 					}
// 				}
// 			}
// 			else if (input[idx] == '|' || input[idx] == ';')
// 			{
// 				if (lexer->data_idx != 0) //今のtoken->dataを閉じる
// 				{
// 					token->data[lexer->data_idx] = '\0';
// 					lexer->data_idx = 0;
// 					//新しく構造体作成
// 					//リストに追加
// 					token = lexer_lstadd(token, input_len);
// 				}
// 				token->data[0] = input[idx];
// 				token->data[1] = '\0';
// 				printf(";[%s]\n", token->data);
// 				if (input[idx + 1] != '\0')
// 				{
// 					token = lexer_lstadd(token, input_len);
// 					lexer->data_idx = 0;
// 				}
// 				else
// 				{
// 					return ;
// 				}
// 			}
// 			else
// 				token->data[lexer->data_idx++] = input[idx];
// 		}
// 		else if (lexer->state == STATE_IN_DQUOTE)
// 			lexer_in_dquote(token, lexer, input[idx]);
// 		else if (lexer->state == STATE_IN_SQUOTE)
// 			lexer_in_squote(token, lexer, input[idx]);
// 		idx++;
// 	}
