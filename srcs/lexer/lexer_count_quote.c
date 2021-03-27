/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_count_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:17:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 21:49:15 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	lexer_count_quote(t_data *data, char *input, char quote)
{
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
	return (0);
}
