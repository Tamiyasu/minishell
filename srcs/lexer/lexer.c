/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/12 20:42:38 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "get_next_line.h"

void	token_print(t_lsttoken *token)
{
	int	i;

	i = 0;
	while (token != NULL)
	{
		printf("token->data[%d]=[%s]\n", i, token->data);
		token = token->next;
	}
}

static t_lsttoken	*lexer_set_token(t_lsttoken *token_list, char *content, int *token_len)
{
	if (token_list == NULL)
	{
		token_list = token_list_new(content);
	}
	else
	{
		t_lsttoken *new_token_list = token_list_new(content);
		token_list_addback(&token_list, new_token_list);
	}
	*token_len = 0;
	return (token_list);
}

t_lsttoken	*lexer_check_tokenlen(t_lsttoken *token_list, char *input, int *start, int *token_len, int *idx)
{
	if (*token_len)
	{
		token_list = lexer_set_token(token_list, ft_substr(&input[*start], 0, *token_len), token_len);
		*start = *idx;
	}
	return (token_list);
}

t_lsttoken	*lexer_set_stdout(t_lsttoken *token_list, char *input, int *token_len, int start, int *idx)
{
	int	i;

	i = 0;
	if (token_len)//check isdigit
	{
		while (i  < *token_len)
		{
			if (ft_isdigit(input[start + i]) != 1)
				break ;
			i++;
		}
		if (i != *token_len)
		{
			token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, *token_len), token_len);
			start = *idx;
		}
	}
	*token_len = (*token_len + 1);//> or >>のcheck
	if (input[*idx + 1] == '>')//さらに>分情報追加
	{
		*token_len = (*token_len + 1);
		*idx = (*idx + 1);
	}
	token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, *token_len), token_len);
	return (token_list);
}

t_lsttoken		*lexer(char *input)
{
	t_lsttoken	*token_list;
	int			token_len;
	int			start;
	int			idx;

	token_list = NULL;
	token_len = 0;
	start = 0;
	idx = 0;
	while (input[idx] != '\0')
	{
		if (input[idx] == '|' || input[idx] == ';' || input[idx] == '<')
		{
			token_list = lexer_check_tokenlen(token_list, input, &start, &token_len, &idx);
			token_len++;
			token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
			start = ++idx;
		}
		else if (input[idx] == '>')
		{
			token_list = lexer_set_stdout(token_list, input, &token_len, start, &idx);
			start = ++idx;
		}
		else if (input[idx] == '\'')
		{
			while (input[idx] != '\0')
			{
				idx++;
				token_len++;
				if (input[idx] == '\'')
				{
					token_len++;
					break ;
				}
			}
			idx++;
		}
		else if (input[idx] == '\"')
		{
			while (input[idx] != '\0')
			{
				idx++;
				token_len++;
				if (input[idx] == '\"' && input[idx - 1] != '\\')
				{
					token_len++;
					break ;
				}
			}
			idx++;
		}
		else if (input[idx] == '\t' || input[idx] == ' ')
		{
			if (token_len)
				token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
			start = ++idx;
		}
		else
		{
			idx++;
			token_len++;
		}
	}
	token_list = lexer_check_tokenlen(token_list, input, &start, &token_len, &idx);
	return (token_list);
}
