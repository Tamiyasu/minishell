/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_set_redirect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:13:23 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 23:15:25 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*check_redirect_digit(t_token *token_list, t_data *data, char *input)
{
	char	*set_token;
	int		i;

	i = 0;
	while (i < data->length)
	{
		if (ft_isdigit(input[data->start + i]) != 1)
			break ;
		i++;
	}
	if (i != data->length)
	{
		set_token = ft_substr(&input[data->start], 0, data->length);
		token_list = lexer_set_token(token_list, data, set_token);
		data->start = data->idx;
	}
	return (token_list);
}

t_token	*lexer_set_redirect(t_token *token_list, t_data *data, char *input)
{
	char	*set_token;

	if (data->length)
		token_list = check_redirect_digit(token_list, data, input);
	data->length = (data->length + 1);
	if (input[data->idx + 1] == '>')
		data_increment(data);
	set_token = ft_substr(&input[data->start], 0, data->length);
	token_list = lexer_set_token(token_list, data, set_token);
	data->start = ++data->idx;
	return (token_list);
}
