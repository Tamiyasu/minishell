/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exps_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:06:37 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/01 20:56:31 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char
	*exps_set_squote_data_1(char *token_data, char *new_data, t_data *data)
{
	new_data = save_reading_data(token_data, new_data, data);
	data->start--;
	data->length++;
	while (token_data[data->idx] != '\'')
	{
		data_increment(data);
	}
	data->length++;
	new_data = save_reading_data(token_data, new_data, data);
	return (new_data);
}

char
	*exps_set_dquote_data_1(t_token *token, char *new_data,
						t_data *data, t_env *env)
{
	new_data = save_reading_data(token->data, new_data, data);
	data->start--;
	data->length++;
	while (token->data[data->idx] != '\"')
	{
		if (token->data[data->idx] == '\\')
		{
			data_increment(data);
			data_increment(data);
		}
		else if (token->data[data->idx] == '$' && token->data[data->idx + 1]
			&& (ft_isalpha(token->data[data->idx + 1])
			|| token->data[data->idx + 1] == '_'
			|| token->data[data->idx + 1] == '?'))
		{
			new_data = exps_set_envdata(token, new_data, data, env);
			token->flag = FT_COMMAND_F;
		}
		else
			data_increment(data);
	}
	data->length++;
	new_data = save_reading_data(token->data, new_data, data);
	return (new_data);
}

char
	*exps_set_squote_data_2(char *token_data, char *new_data, t_data *data)
{
	new_data = save_reading_data(token_data, new_data, data);
	while (token_data[data->idx] != '\'')
	{
		data_increment(data);
	}
	new_data = save_reading_data(token_data, new_data, data);
	return (new_data);
}

char
	*exps_set_dquote_data_2(t_token *token, char *new_data, t_data *data)
{
	new_data = save_reading_data(token->data, new_data, data);
	while (token->data[data->idx] != '\"')
	{
		if (token->data[data->idx] == '\\' &&
			(token->data[data->idx + 1] == '\\'
			|| token->data[data->idx + 1] == '\"'
			|| token->data[data->idx + 1] == '$'))
		{
			new_data = save_reading_data(token->data, new_data, data);
			data_increment(data);
		}
		else
			data_increment(data);
	}
	new_data = save_reading_data(token->data, new_data, data);
	return (new_data);
}
