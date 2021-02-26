/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_setchr_and_next.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/26 16:55:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/26 17:57:08 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/*
** token->dataに何かしら文字が入っていたら、'\0'を入れる。
** 新しいリスト構造体にinput_cを入れて、その後ろに新しいリスト構造体を加えて戻す。
*/
t_lsttoken	*lexer_setchr_and_next(t_lsttoken *token, t_lexer *lexer, char input_c, int input_len)
{
	if (lexer->data_idx != 0 || input_c == 0) //今のtoken->dataを閉じる
	{
		token->data[lexer->data_idx] = '\0';
		lexer->data_idx = 0;
		token = lexer_lstadd(token, input_len);
	}
	if (input_c == 0)
	{
		return (token);
	}
	token->data[0] = input_c;
	token->data[1] = '\0';
	return (lexer_lstadd(token, input_len));
}
