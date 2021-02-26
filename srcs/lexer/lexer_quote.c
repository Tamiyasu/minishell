/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 17:36:34 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/26 20:19:37 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lsttoken	*lexer_in_dquote(t_lsttoken *token, t_lexer *lexer, char input_c)
{
	token->data[lexer->data_idx++] = input_c;
	if (input_c == CHAR_DQUOTE)
	{
		if (token->data[lexer->data_idx - 2] != '\\')
		{
			lexer->state = STATE_GENERAL;
		}
	}
	return (token);
}

t_lsttoken	*lexer_in_squote(t_lsttoken *token, t_lexer *lexer, char input_c)
{
	token->data[lexer->data_idx++] = input_c;
	if (input_c == CHAR_SQUOTE)
	{
		lexer->state = STATE_GENERAL;
	}
	return (token);
}

t_lsttoken	*lexer_set_quote(t_lsttoken *token, t_lexer *lexer, char input_c)
{
	if (input_c == '\'')
	{
		lexer->state = STATE_IN_SQUOTE;
	}
	else
	{
		lexer->state = STATE_IN_DQUOTE;
	}
	token->data[lexer->data_idx++] = input_c;
	return (token);
}
