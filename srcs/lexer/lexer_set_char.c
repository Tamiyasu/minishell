/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_set_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:35:15 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 17:35:27 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*lexer_set_char(t_token *token_list, t_lexer *lexer, char *input)
{
	char	*set_token;

	token_list = lexer_check_tokenlen(token_list, lexer, input);
	lexer->token_len++;
	set_token = ft_substr(&input[lexer->start], 0, lexer->token_len);
	token_list = lexer_set_token(token_list, lexer, set_token);
	lexer->start = ++lexer->idx;
	return (token_list);
}
