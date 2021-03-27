/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 21:27:00 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int		error_token(t_token *token, char *err_str, t_data *data)
{
	error_str("'");
	error_str(err_str);
	error_str("syntax error near unexpected token `");
	free(data);
	free_lst(&token);
	return (0);
}

int		lexer_increment(t_token *token, char *input, t_data *data)
{
	if (input[data->idx] == '\'' || input[data->idx] == '\"')
	{
		if (!lexer_count_quote(data, input, input[data->idx]))
			return (error_token(token, &input[data->idx], data));
		return (1);
	}
	if (input[data->idx] == '\\')
	{
		data_increment(data);
		if (input[data->idx] == '\0')
			return (error_token(token, "\\", data));
	}
	data_increment(data);
	return (1);
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
//	printf("input : [%s]\n", input);
	while (input[data->idx] != '\0')
	{
		printf("data->idx [%d]: input[data->idx] [%c]\n", data->idx, input[data->idx]);
		if (input[data->idx] == '|' || input[data->idx] == ';')
			token_list = lexer_set_char(token_list, data, input);
		else if (input[data->idx] == '<' || input[data->idx] == '>')
			token_list = lexer_set_redirect(token_list, data, input);
		else if (input[data->idx] == '\t' || input[data->idx] == ' ')
			token_list = lexer_skip_space(token_list, data, input);
		else
		{
			if (!lexer_increment(token_list, input, data))
				return (0);
		}
		printf("data->idx [%d]\n", data->idx);
	}
	token_list = lexer_check_len(token_list, data, input);
	free(data);
	*token_list_p = token_list;
	return (1);
}
