/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_set_redirect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 16:50:01 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 15:53:43 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static t_lexer_token	*set_redirect_and_next(t_lexer_token *token, t_lexer *lexer, int num, int input_len)
{
	if (num == 2)
	{
		ft_memcpy(token->data, ">>", 3);
		return (lexer_lstadd(token, input_len));
	}
	return (lexer_setchr_and_next(token, lexer, '>', input_len));
}

t_lexer_token	*lexer_set_redirect(t_lexer_token *token, t_lexer *lexer, char *input, int *idx,  int input_len)
{
	int	i;
	int	num;

	num = 1;
	if (input[*idx] == '>' && input[*idx + 1] == '>')
	{
		*idx = *idx + 1;
		num++;
	}
	if  (input[*idx] == '<')
	{
		if (lexer->data_idx != 0)
		{
			token->data[lexer->data_idx] = '\0';
			lexer->data_idx = 0;
			token =  lexer_lstadd(token, input_len);
		}
		token->data[0] = '<';
		token->data[1] = '\0';
		if (input[*idx + 1] != '\0')
		{
			token = lexer_lstadd(token, input_len);
			lexer->data_idx = 0;
		}
		return (token);
		// 	return (lexer_setchr_and_next(token, lexer, input[*idx], input_len));
		// }
		// else
		// {
		// 	if (lexer->data_idx != 0)
		// 	{
		// 		token->data[lexer->data_idx] = '\0';
		// 		lexer->data_idx = 0;
		// 		token = lexer_lstadd(token, input_len);
		// 	}
		// }
		// token->data[0] = '<';
		// token->data[1] = '\0';
		// return (token);
	}
	if (lexer->data_idx == 0)//token->data[0]>入れて終了
	{
		return (set_redirect_and_next(token, lexer, num, input_len));
	}
	i  =  0; //>の前の文字列と分離させるかくっつけるか評価
	while (i < lexer->data_idx)
	{
		if (ft_isdigit(token->data[i]) != 1)
		{
			token = lexer_setchr_and_next(token, lexer, 0, input_len);
			return (set_redirect_and_next(token, lexer, num, input_len));
		}
		i++;
	}
	if (num == 2)
	{
		token->data[lexer->data_idx++] = '>';
		token->data[lexer->data_idx++] = '>';
		return (lexer_setchr_and_next(token, lexer, 0, input_len));
	}
	token->data[lexer->data_idx++] = '>';
	return (lexer_setchr_and_next(token, lexer, 0, input_len));
}
