/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 22:28:58 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include "get_next_line.h"

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

int			check_redirect_num()
{

}

t_lsttoken	*lexer_set_stdout(t_lsttoken *token_list, char *input, int *token_len, int *start, int *idx)
{
	t_lsttoken	*new_token_list;
	char		*redirect;
	char		*maybe_fd;
	int			flag_fd = 1;

	if (input[*idx + 1] ==  '>')
	{
		redirect = ft_strdup(">>");
		*idx = (*idx + 1);
	}
	else
	{
		redirect = ft_strdup(">");
	}
	if (token_len)//'<'の前に文字列あったら処理→前の文字列がすべてisdigitなら、くっつけてあげる
	{
		int i = 0;
		maybe_fd = ft_substr(&input[*start], 0, *token_len);
		while (maybe_fd[i] != '\0')
		{
			if (ft_isdigit(maybe_fd[i]) != 1)
			{
				flag_fd = 0;
				break ;
			}
			i++;
		}
		if (flag_fd)//くっつけて入れて終了
		{
			token_list = lexer_set_token(token_list, ft_strjoin(maybe_fd, redirect), token_len);
		}
		else//別々に入れる
		{
			token_list = lexer_set_token(token_list, ft_strdup(maybe_fd), token_len);
			new_token_list = token_list_new(ft_strdup(redirect));
			token_list_addback(&token_list, new_token_list);
		}
		free(maybe_fd);
		free(redirect);
	}
	else
	{
		token_len++;
		token_list = lexer_set_token(token_list, ft_strdup(redirect), token_len);
		free(redirect);
	}
	*idx = (*idx + 1);
	*start = *idx;
	return (token_list);
}
/*
** トークン(意味のある単語)に分ける
*/
t_lsttoken	*msh_lexer(char *input)
{
	t_lsttoken *token_list;
	int		idx;
	int		start;
	int		token_len;

	token_list = NULL;
	idx = 0;
	start = 0;
	token_len = 0;
	while (input[idx] != '\0')
	{
		if (input[idx] == '|' || input[idx] == ';' || input[idx] == '<')
		{
			if (token_len)
			{
				token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
			}
			start = idx;
			token_len++;
			token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
			start = ++idx;
		}
		else if (input[idx] == '>')
		{
			token_list = lexer_set_stdout(token_list, input, &token_len, &start, &idx);
		}
		else if (input[idx] == '\'')
		{
			if (token_len)
			{
				token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
			}
			start = idx;
			while (input[idx] != '\0')
			{
				idx++;
				token_len++;
				if (input[idx] == '\'')//ここまでをnew_listに追加
				{
					token_len++;
					token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
					start = ++idx;
					break ;
				}
			}
		}
		else if (input[idx] == '\"')
		{
			if (token_len)
			{
				token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
			}
			start = idx;
			while (input[idx] != '\0')
			{
				idx++;
				token_len++;
				if (input[idx] == '\"' && input[idx - 1] != '\\')//ここまでをnew_listに追加
				{
					token_len++;
					token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
					start = ++idx;
					break ;
				}
			}
		}
		else if (input[idx] == '\t' || input[idx] == ' ')
		{
			if (token_len)
			{
				token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
			}
			start = ++idx;
		}
		else
		{
			idx++;
			token_len++;
		}
	}
	if (token_len)
	{
		token_list = lexer_set_token(token_list, ft_substr(&input[start], 0, token_len), &token_len);
	}
	return (token_list);
}
