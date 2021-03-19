/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 23:36:40 by ysaito           ###   ########.fr       */
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

t_token	*lexer_set_token(t_token *token_list, t_data *data, char *content)
{
	t_token *new_token_list;

	if (token_list == NULL)
		token_list = token_list_new(content);
	else
	{
		new_token_list = token_list_new(content);
		token_list_addback(&token_list, new_token_list);
	}
	data->length = 0;
	return (token_list);
}

t_token	*lexer_check_tokenlen(t_token *token_list, t_data *data, char *input)
{
	char	*set_token;

	if (data->length)
	{
		set_token = ft_substr(&input[data->start], 0, data->length);
		token_list = lexer_set_token(token_list, data, set_token);
		data->start = data->idx;
	}
	return (token_list);
}

int	lexer(char *input, t_token **token_list_p)
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
			{
				error_str("'");
				error_str(&input[data->idx]);
				error_str("quotation marks error near token `");
				free_lst(&token_list);
				g_exit_status = EXIT_SYNTAX_ERROR;
				return (0);
			}
		}
		else if (input[data->idx] == '\t' || input[data->idx] == ' ')
			token_list = lexer_skip_space(token_list, data, input);
		else
			data_increment(data);
	}
	token_list = lexer_check_tokenlen(token_list, data, input);
	free(data);
	*token_list_p = token_list;
	return (1);
}
