/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_count_quote.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 17:17:38 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 23:22:23 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	lexer_count_quote(t_data *data, char *input, char quote)
{
	int	save_idx;

	save_idx = data->idx;
	while (input[data->idx] != '\0')
	{
		data->idx++;
		data->length++;
		if (input[data->idx] == quote)
		{
			data->length++;
			return (1) ;
		}
	}
	data->idx = save_idx;
	return (0);
}
