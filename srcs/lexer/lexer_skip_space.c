/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_skip_space.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:33:15 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 17:33:41 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*lexer_skip_space(t_token *token_list, t_lexer *lexer, char *input)
{
	char	*set_token;

	if (lexer->token_len)
	{
		set_token = ft_substr(&input[lexer->start], 0, lexer->token_len);
		token_list = lexer_set_token(token_list, lexer, set_token);
	}
	lexer->start = ++lexer->idx;
	return (token_list);
}
