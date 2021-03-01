/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/01 15:43:23 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "get_next_line.h"

static void			lexer_structinit(t_lexer *lexer)
{
	lexer->data_idx = 0;
	lexer->state = STATE_GENERAL;
}

static t_lsttoken *lexer_skip_space(t_lsttoken *token, t_lexer *lexer, char *input, int *idx, int input_len)
{
	while (input[*idx] == ' ' || input[*idx] == '\t')
	{
		*idx = *idx + 1;
	}
	if (input[*idx] == '\0')
	{
		//break ;
		return (token);
	}
	if (lexer->data_idx != 0)
	{
		token = lexer_setchr_and_next(token, lexer, 0, input_len);
	}
	return (token);
}

/*
** inputを１文字ずつ評価
*/
static void	lexer_evaluate_input(t_lsttoken *token, t_lexer *lexer, char *input, int input_len)
{
	int	idx;

	token = lexer_lstlast(token);
	idx = 0;
	while (input[idx] != '\0')
	{
		if (lexer->state == STATE_GENERAL)
		{
			if (input[idx] == '\'' || input[idx] == '\"')
			{
				token = lexer_set_quote(token, lexer, input[idx]);
			}
			else if (input[idx] == ' ' || input[idx] == '\t')
			{
				token = lexer_skip_space(token, lexer, input, &idx, input_len);
				continue ;
			}
			else if (input[idx] == '<' || input[idx]  == '>')
			// {
			// 	if (input[idx] == '>' && input[idx + 1] == '>')
			// 	{
			// 		idx++;
			// 		token = lexer_set_redirect(token, lexer, input[idx], 2, input_len);
			// 	}
			// 	else
			// 	{
			// 		token = lexer_set_redirect(token, lexer, input[idx], 1, input_len);
			// 	}
			// }
			{
				token = lexer_set_redirect(token, lexer, input, &idx, input_len);
			}
			else if (input[idx] == '|' || input[idx] == ';')
			{
				token = lexer_setchr_and_next(token, lexer, input[idx], input_len);
			}
			else
			{
				token->data[lexer->data_idx++] = input[idx];
			}
		}
		else if (lexer->state == STATE_IN_DQUOTE)
		{
			lexer_in_dquote(token, lexer, input[idx]);
		}
		else if (lexer->state == STATE_IN_SQUOTE)
		{
			lexer_in_squote(token, lexer, input[idx]);
		}
		idx++;
	}
	token->data[lexer->data_idx] = '\0';
}

/*
** トークン(意味のある単語)に分ける
*/
t_lsttoken	*lexer(char *input)
{
	t_lsttoken *token;
	t_lexer		lexer;
	size_t	input_len;

	input_len = ft_strlen(input); /* inputの長さを求める */
	token = lexer_lstnew(input_len); /* t_lsttoken　mallocして初期化 */
	if (token == NULL)//lexer_lstnew内のmallocで失敗した時
	{
		return (NULL);
	}
	lexer_structinit(&lexer); /*msh_lexer.c内で使う構造体の初期化 */
	lexer_evaluate_input(token, &lexer, input, input_len);
	if (token == NULL)
	{
		free_lst(&token);
		return (NULL);
	}
	return (token);
}
