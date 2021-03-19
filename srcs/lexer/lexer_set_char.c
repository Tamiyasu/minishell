/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_set_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:35:15 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 23:13:17 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*lexer_set_char(t_token *token_list, t_data *data, char *input)
{
	char	*set_token;

	token_list = lexer_check_tokenlen(token_list, data, input);
	data->length++;
	set_token = ft_substr(&input[data->start], 0, data->length);
	token_list = lexer_set_token(token_list, data, set_token);
	data->start = ++data->idx;
	return (token_list);
}
