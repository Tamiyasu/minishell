/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/01/17 21:01:47 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "get_next_line.h"

void	lexer_lstinit(t_lsttoken *token, int input_len)
{
	token->data = malloc(sizeof(char *) * (input_len + 1));
	if (token->data == NULL)
		return ;
	ft_bzero(token->data, input_len);
	token->next = NULL;
}

static t_lsttoken	*lexer_lstnew(int input_len)
{
	t_lsttoken	*return_p;

	return_p = malloc(sizeof(t_lsttoken));
	if (return_p)
	{
		return_p->next = 0;
		return_p->data = malloc(sizeof(char *) * (input_len + 1));
		if (return_p->data == NULL)
			return (NULL);
	}
	ft_bzero(return_p->data, input_len);
	return (return_p);
}

// static void	lexer_lstadd(t_lsttoken *token, int input_len)
// {
// 	while (token->next != NULL)
// 	{
// 		token = token->next;
// 	}
// 	token->next = lexer_lstnew(input_len);
// }


/*
** トークン(意味のある単語)に分ける
*/
t_lsttoken	*msh_lexer(char *input)
{
	t_lsttoken *token;
	t_lsttoken *savetoken;
	size_t	input_len;
	int	state;
	int	idx;
	int	data_idx;
	int	squote;

	//inputの長さを求める
	input_len = ft_strlen(input);
	token = lexer_lstnew(input_len);
	savetoken = token;
	state = STATE_GENERAL;

	idx = 0;
	data_idx = 0;
	squote = 0;
	//inputを１文字ずつ評価
	while (input[idx] != '\0')
	{
		if (state == STATE_GENERAL)
		{
			if (input[idx] == '\'')
			{
				state = STATE_IN_SQUOTE;
				squote++;
			}
			//'\""(double quote)がきたら //'$', '\\'以外は文字列リテラルとされる
			else if (input[idx] == '\"')
			{
				state = STATE_IN_DQUOTE;
			}
			else if (input[idx] == '\\')
			{
				idx++;
				token->data[data_idx++] = input[idx];
			}
			else if (input[idx] == ' ' || input[idx] == '\t')
			{
				if (data_idx != 0)
				{
					token->data[data_idx] = '\0';
					token->next = malloc(sizeof(t_lsttoken));
					token = token->next;
					lexer_lstinit(token, input_len);
					data_idx = 0;
				}
			}
			//'|', ';', '<', '>', '=' がきたら
			else if (input[idx] == '|' || input[idx] == ';'
					|| input[idx] == '<' || input[idx]  == '>' || input[idx] == '=')
			{
				if  (data_idx != 0)
				{
					token->data[data_idx] = '\0';
					token->next = malloc(sizeof(t_lsttoken));
					token = token->next;
					lexer_lstinit(token, input_len);
					data_idx = 0;
				}
				token->data[0] = input[idx];
				token->data[1] = '\0';

				token->next = malloc(sizeof(t_lsttoken));
				token = token->next;
				lexer_lstinit(token, input_len);
			}
			//それ以外の文字
			else
			{
				token->data[data_idx++] = input[idx];
			}
		}
		else if (state == STATE_IN_DQUOTE)
		{
			if (input[idx] == CHAR_DQUOTE)
			{
				state = STATE_GENERAL;
				idx++;
				continue;
			}
			token->data[data_idx++] = input[idx];
		}
		else if (state == STATE_IN_SQUOTE)
		{
			if (input[idx] == CHAR_SQUOTE)
			{
				state = STATE_GENERAL;
				squote++;
				idx++;
				continue;
			}
			token->data[data_idx++] = input[idx];
		}
		idx++;
	}
	token->data[data_idx] = '\0';
	return (savetoken);
}
