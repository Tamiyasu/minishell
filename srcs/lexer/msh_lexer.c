/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 17:09:23 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "get_next_line.h"


// static void			lexer_structinit(t_lexer *lexer)
// {
// 	lexer->data_idx = 0;
// 	lexer->state = STATE_GENERAL;
// }

// static t_lexer_token *lexer_skip_space(t_lexer_token *token, t_lexer *lexer, char *input, int *idx, int input_len)
// {
// 	while (input[*idx] == ' ' || input[*idx] == '\t')
// 	{
// 		*idx = *idx + 1;
// 	}
// 	if (input[*idx] == '\0')
// 	{
// 		return (token);
// 	}
// 	if (lexer->data_idx != 0)
// 	{
// 		token = lexer_setchr_and_next(token, lexer, 0, input_len);
// 	}
// 	return (token);
// }

/*
// ** inputを１文字ずつ評価
*/
// static void	lexer_evaluate_input(t_lexer_token *token, t_lexer *lexer, char *input, int input_len)
// {
// 	int	idx;

// 	token = lexer_lstlast(token);
// 	idx = 0;
// 	while (input[idx] != '\0')
// 	{
// 		if (lexer->state == STATE_GENERAL)
// 		{
// 			if (input[idx] == '\'' || input[idx] == '\"')
// 				token = lexer_set_quote(token, lexer, input[idx]);
// 			else if (input[idx] == ' ' || input[idx] == '\t')
// 			{
// 				token = lexer_skip_space(token, lexer, input, &idx, input_len);
// 				continue ;
// 			}
// 			else if (input[idx] == '<' || input[idx]  == '>')
// 			{
// 				//token = lexer_set_redirect(token, lexer, input, &idx, input_len);
// 				if  (input[idx] == '<')
// 				{
// 					if (lexer->data_idx != 0)
// 					{
// 						token->data[lexer->data_idx] = '\0';
// 						lexer->data_idx = 0;
// 						token =  lexer_lstadd(token, input_len);
// 					}
// 					token->data[0] = '<';
// 					token->data[1] = '\0';
// 					if (input[idx + 1] != '\0')
// 					{
// 						token = lexer_lstadd(token, input_len);
// 						lexer->data_idx = 0;
// 					}
// 				}
// 			}
// 			else if (input[idx] == '|' || input[idx] == ';')
// 			{
// 				if (lexer->data_idx != 0) //今のtoken->dataを閉じる
// 				{
// 					token->data[lexer->data_idx] = '\0';
// 					lexer->data_idx = 0;
// 					//新しく構造体作成
// 					//リストに追加
// 					token = lexer_lstadd(token, input_len);
// 				}
// 				token->data[0] = input[idx];
// 				token->data[1] = '\0';
// 				printf(";[%s]\n", token->data);
// 				if (input[idx + 1] != '\0')
// 				{
// 					token = lexer_lstadd(token, input_len);
// 					lexer->data_idx = 0;
// 				}
// 				else
// 				{
// 					return ;
// 				}
// 			}
// 			else
// 				token->data[lexer->data_idx++] = input[idx];
// 		}
// 		else if (lexer->state == STATE_IN_DQUOTE)
// 			lexer_in_dquote(token, lexer, input[idx]);
// 		else if (lexer->state == STATE_IN_SQUOTE)
// 			lexer_in_squote(token, lexer, input[idx]);
// 		idx++;
// 	}
// 	token->data[lexer->data_idx] = '\0';
// 	return ;
// }

/*
** トークン(意味のある単語)に分ける
*/
t_lexer_token	*lexer(char *input)
{
	t_lexer_token *token_list;
	//t_lexer		lexer;
	//size_t	input_len;
	//char	*token;
	int		idx;
	int		start;
	int		token_len;
	// int		status;

	//input_len = ft_strlen(input); /* inputの長さを求める */
	//token = lexer_lstnew(input_len); /* t_lsttoken　mallocして初期化 */
	// if (token == NULL)//lexer_lstnew内のmallocで失敗した時
	// {
	// 	return (NULL);
	// }
	//lexer_structinit(&lexer); /*msh_lexer.c内で使う構造体の初期化 */
	//lexer_evaluate_input(token, &lexer, input, input_len);

	token_list = NULL;
	idx = 0;
	start = 0;
	token_len = 0;
	while (input[idx] != '\0')
	{
		if (input[idx] == '\'')
		{
			if (token_len)//'\''の前に文字列あったら処理
			{
				if (token_list == NULL)
				{
					token_list = token_list_new(ft_substr(&input[start], 0, token_len));
				}
				else
				{
					t_lexer_token *new_token_list = token_list_new(ft_substr(&input[start], 0, token_len));
					token_list_addback(&token_list, new_token_list);
				}
				token_len = 0;
			}
			start = idx;
			while (input[idx] != '\0')
			{
				idx++;
				token_len++;
				if (input[idx] == '\'')//ここまでをnew_listに追加
				{
					idx++;
					token_len++;
					if (token_list == NULL)
					{
						token_list = token_list_new(ft_substr(&input[start], 0, token_len));
					}
					else
					{
						t_lexer_token *new_token_list = token_list_new(ft_substr(&input[start], 0, token_len));
						token_list_addback(&token_list, new_token_list);
					}
					token_len = 0;
					break ;
				}
			}
		}
		else if (input[idx] == '\"')
		{
			if (token_len)//'\"の前に文字列あったら処理
			{
				if (token_list == NULL)
				{
					token_list = token_list_new(ft_substr(&input[start], 0, token_len));
				}
				else
				{
					t_lexer_token *new_token_list = token_list_new(ft_substr(&input[start], 0, token_len));
					token_list_addback(&token_list, new_token_list);
				}
				token_len = 0;
			}
			start = idx;
			while (input[idx] != '\0')
			{
				idx++;
				token_len++;
				if (input[idx] == '\"')//ここまでをnew_listに追加
				{
					idx++;
					token_len++;
					if (token_list == NULL)
					{
						token_list = token_list_new(ft_substr(&input[start], 0, token_len));
					}
					else
					{
						t_lexer_token *new_token_list = token_list_new(ft_substr(&input[start], 0, token_len));
						token_list_addback(&token_list, new_token_list);
					}
					token_len = 0;
					break ;
				}
			}
		}
		else if (input[idx] == '\t' || input[idx] == ' ')
		{
			if (token_len)
			{
				if (token_list == NULL)
				{
					token_list = token_list_new(ft_substr(&input[start], 0, token_len));
				}
				else
				{
					t_lexer_token *new_token_list = token_list_new(ft_substr(&input[start], 0, token_len));
					token_list_addback(&token_list, new_token_list);
				}
				token_len = 0;
			}
			idx++;
			start = idx;
		}
		else
		{
			idx++;
			token_len++;
		}
	}
	if (token_len)
	{
		if (token_list == NULL)
		{
			token_list = token_list_new(ft_substr(&input[start], 0, token_len));
		}
		else
		{
			t_lexer_token *new_token_list = token_list_new(ft_substr(&input[start], 0, token_len));
			token_list_addback(&token_list, new_token_list);
		}
		token_len = 0;
	}

	// if (token == NULL)
	// {
	// 	free_lst(&token);
	// 	return (NULL);
	// }
	return (token_list);
}
