/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 17:41:09 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	lexer_increment(t_lexer *lexer)
{
	lexer->idx++;
	lexer->token_len++;
}

t_lexer	*lexer_init(void)
{
	t_lexer *lexer;

	lexer = malloc(sizeof(t_lexer));
	if (lexer)
	{
		lexer->token_len = 0;
		lexer->start = 0;
		lexer->idx = 0;
	}
	return (lexer);
}

t_token	*lexer_set_token(t_token *token_list, t_lexer *lexer, char *content)
{
	t_token *new_token_list;

	if (token_list == NULL)
		token_list = token_list_new(content);
	else
	{
		new_token_list = token_list_new(content);
		token_list_addback(&token_list, new_token_list);
	}
	lexer->token_len = 0;
	return (token_list);
}

t_token	*lexer_check_tokenlen(t_token *token_list, t_lexer *lexer, char *input)
{
	char	*set_token;

	if (lexer->token_len)
	{
		set_token = ft_substr(&input[lexer->start], 0, lexer->token_len);
		token_list = lexer_set_token(token_list, lexer, set_token);
		lexer->start = lexer->idx;
	}
	return (token_list);
}

t_token	*lexer(char *input)
{
	t_token	*token_list;
	t_lexer	*lexer;

	token_list = NULL;
	lexer = lexer_init();
	while (input[lexer->idx] != '\0')
	{
		if (input[lexer->idx] == '|' || input[lexer->idx] == ';')
			token_list = lexer_set_char(token_list, lexer, input);
		else if (input[lexer->idx] == '<' || input[lexer->idx] == '>')
			token_list = lexer_set_redirect(token_list, lexer, input);
		else if (input[lexer->idx] == '\'')
			lexer_squote_count_tokenlen(lexer, input);
		else if (input[lexer->idx] == '\"')
			lexer_dquote_count_tokenlen(lexer, input);
		else if (input[lexer->idx] == '\t' || input[lexer->idx] == ' ')
			token_list = lexer_skip_space(token_list, lexer, input);
		else
			lexer_increment(lexer);
	}
	token_list = lexer_check_tokenlen(token_list, lexer, input);
	free(lexer);
	return (token_list);
}
