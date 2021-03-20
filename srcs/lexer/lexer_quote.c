/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:17:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 15:08:28 by ysaito           ###   ########.fr       */
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
	while (input[data->idx] != '\0')
	{
		data_increment(data);
		if (input[data->idx] == quote)
		{
			data_increment(data);
			return (1);
		}
	}
	data->idx = save_idx;
	return (0);
}
