/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exps_format.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:27:11 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 20:26:17 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int		is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

void	exps_free(t_data *data, char *tmp_token_data)
{
	free(tmp_token_data);
	tmp_token_data = NULL;
	free(data);
	data = NULL;
}

void	set_token(t_token *token)
{
	free(token->data);
	token->data = NULL;
	token->flag = FT_COMMAND_F;
}

t_token	*set_format_data(t_token *token, char *tmp_token_data, t_data *data)
{
	t_token	*add_token;
	char	*format_data;

	if (data->length)
	{
		if (token->data == NULL)
			token->data = ft_substr(tmp_token_data, data->start, data->length);
		else
		{
			format_data = ft_substr(tmp_token_data, data->start, data->length);
			add_token = token_list_new(format_data);
			add_token->flag = FT_COMMAND_F;
			add_token->next = token->next;
			token->next = add_token;
		}
		data->length = 0;
	}
	data->start = ++data->idx;
	return (token);
}

t_token	*exps_format(t_token *token)
{
	t_token	*token_p;
	t_data	*data;
	char	*tmp_token_data;

	token_p = token;
	while (token)
	{
		if (token->flag == FT_EXPANSION_ENV && ft_strcmp(token->data, "") != 0)
		{
			data = data_init();
			tmp_token_data = ft_strdup(token->data);
			set_token(token);
			while (tmp_token_data[data->idx])
			{
				if (is_space(tmp_token_data[data->idx]))
					token = set_format_data(token, tmp_token_data, data);
				else
					data_increment(data);
			}
			token = set_format_data(token, tmp_token_data, data);
			exps_free(data, tmp_token_data);
		}
		token = token->next;
	}
	return (token_p);
}
