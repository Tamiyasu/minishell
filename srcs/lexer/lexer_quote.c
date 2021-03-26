/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:17:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/26 19:59:33 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	error_quote(t_token *token, char *input, t_data *data)
{
	error_str("'");
	error_str(&input[data->idx]);
	error_str("quotation marks error near token `");
	free(data);
	free_lst(&token);
	return (0);
}

int	lexer_count_quote(t_data *data, char *input, char quote)
{
	int	save_idx;

	save_idx = data->idx;
	data_increment(data);
	while (input[data->idx] != '\0')
	{
		if (quote == '\"' &&
			((input[data->idx] == '\\' && input[data->idx + 1] == '\"')
			|| (input[data->idx] == '\\' && input[data->idx + 1] == '\\')))
		{
			data_increment(data);
			data_increment(data);
		}
		else if (input[data->idx] == quote)
		{
			data_increment(data);
			return (1);
		}
		else
			data_increment(data);
	}
	data->idx = save_idx;
	return (0);
}
