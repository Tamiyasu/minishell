/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 17:36:34 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 15:53:43 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_lexer_token	*lexer_in_dquote(t_lexer_token *token, t_lexer *lexer, char input_c)
{
	token->data[lexer->data_idx++] = input_c;
	if (input_c == '\"')
	{
		if (token->data[lexer->data_idx - 2] != '\\')
		{
			lexer->state = STATE_GENERAL;
		}
	}
	return (token);
}

t_lexer_token	*lexer_in_squote(t_lexer_token *token, t_lexer *lexer, char input_c)
{
	token->data[lexer->data_idx++] = input_c;
	if (input_c == '\'')
	{
		lexer->state = STATE_GENERAL;
	}
	return (token);
}

t_lexer_token	*lexer_set_quote(t_lexer_token *token, t_lexer *lexer, char input_c)
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
