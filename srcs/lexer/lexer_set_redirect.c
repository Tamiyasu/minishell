/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:13:23 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 17:39:53 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*check_redirect_digit(t_token *token_list, t_lexer *lexer, char *input)
{
	char	*set_token;
	int		i;

	i = 0;
	while (i < lexer->token_len)
	{
		if (ft_isdigit(input[lexer->start + i]) != 1)
			break ;
		i++;
	}
	if (i != lexer->token_len)
	{
		set_token = ft_substr(&input[lexer->start], 0, lexer->token_len);
		token_list = lexer_set_token(token_list, lexer, set_token);
		lexer->start = lexer->idx;
	}
	return (token_list);
}

t_token	*lexer_set_redirect(t_token *token_list, t_lexer *lexer, char *input)
{
	char	*set_token;

	if (lexer->token_len)
		token_list = check_redirect_digit(token_list, lexer, input);
	lexer->token_len = (lexer->token_len + 1);
	if (input[lexer->idx + 1] == '>')
		lexer_increment(lexer);
	set_token = ft_substr(&input[lexer->start], 0, lexer->token_len);
	token_list = lexer_set_token(token_list, lexer, set_token);
	lexer->start = ++lexer->idx;
	return (token_list);
}
