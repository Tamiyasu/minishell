/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_count_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:17:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 21:22:14 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
			printf("data-idx:[%d]\n", data->idx);
			printf("data-idx:[%d]/input[data-idx][%c]\n", data->idx, input[data->idx]);
			return (1);
		}
		else
			data_increment(data);
	}
	data->idx = save_idx;
	return (0);
}
