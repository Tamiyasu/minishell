/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exps_increment.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/03 18:09:25 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/03 18:09:36 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	exps_increment_1(t_token *token, t_data *data)
{
	if (token->data[data->idx] == '\\')
		data_increment(data);
	data_increment(data);
}

char	*exps_increment_2(t_token *token, char *new_data, t_data *data)
{
	if (token->data[data->idx] == '\\')
		new_data = save_reading_data(token->data, new_data, data);
	data_increment(data);
	return (new_data);
}
