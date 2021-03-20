/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 15:09:37 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	data_increment(t_data *data)
{
	data->idx++;
	data->length++;
}

t_data	*data_init(void)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if (data)
	{
		data->length = 0;
		data->start = 0;
		data->idx = 0;
	}
	return (data);
}

t_token	*lexer_set_token(t_token *token, t_data *data, char *content)
{
	t_token *new_token_list;

	if (token == NULL)
		token = token_list_new(content);
	else
	{
		new_token_list = token_list_new(content);
		token_list_addback(&token, new_token_list);
	}
	data->length = 0;
	return (token);
}

t_token	*lexer_check_len(t_token *token, t_data *data, char *input)
{
	char	*set_token;

	if (data->length)
	{
		set_token = ft_substr(&input[data->start], 0, data->length);
		token = lexer_set_token(token, data, set_token);
		data->start = data->idx;
	}
	return (token);
}

int		lexer(char *input, t_token **token_list_p)
{
	t_token	*token_list;
	t_data	*data;

	token_list = NULL;
	data = data_init();
	while (input[data->idx] != '\0')
	{
		if (input[data->idx] == '|' || input[data->idx] == ';')
			token_list = lexer_set_char(token_list, data, input);
		else if (input[data->idx] == '<' || input[data->idx] == '>')
			token_list = lexer_set_redirect(token_list, data, input);
		else if (input[data->idx] == '\'' || input[data->idx] == '\"')
		{
			if (!lexer_count_quote(data, input, input[data->idx]))
				return (error_quote(token_list, input, data));
		}
		else if (input[data->idx] == '\t' || input[data->idx] == ' ')
			token_list = lexer_skip_space(token_list, data, input);
		else
			data_increment(data);
	}
	token_list = lexer_check_len(token_list, data, input);
	free(data);
	*token_list_p = token_list;
	return (1);
}
