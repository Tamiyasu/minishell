/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:17:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 17:18:46 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	lexer_squote_count_tokenlen(t_lexer *lexer, char *input)
{
	while (input[lexer->idx] != '\0')
	{
		lexer->idx = (lexer->idx + 1);
		lexer->token_len++;
		if (input[lexer->idx] == '\'')
		{
			lexer->token_len++;
			break ;
		}
	}
	lexer->idx++;
}

void	lexer_dquote_count_tokenlen(t_lexer *lexer, char *input)
{
	while (input[lexer->idx] != '\0')
	{
		lexer->idx = (lexer->idx + 1);
		lexer->token_len++;
		if (input[lexer->idx] == '\"' && input[lexer->idx - 1] != '\\')
		{
			lexer->token_len++;
			break ;
		}
	}
	lexer->idx++;
}
