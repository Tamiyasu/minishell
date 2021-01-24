/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_lexer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/13 16:38:10 by ysaito            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/01/17 21:01:47 by ysaito           ###   ########.fr       */
=======
/*   Updated: 2021/01/24 17:42:42 by ysaito           ###   ########.fr       */
>>>>>>> yuka-dir
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include "get_next_line.h"

<<<<<<< HEAD
void	lexer_lstinit(t_lsttoken *token, int input_len)
=======
static void			lexer_structinit(t_lexer *lexer)
{
	lexer->data_idx = 0;
	lexer->squote = 0;
	lexer->dquote = 0;
	lexer->state = STATE_GENERAL;
}

static void			lexer_lstinit(t_lsttoken *token, int input_len)
>>>>>>> yuka-dir
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
<<<<<<< HEAD
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
=======
	if (return_p == NULL)
		return (NULL);
	lexer_lstinit(return_p, input_len);
	if  (return_p->data == NULL)
	{
		free(return_p);
		return (NULL);
	}
	return (return_p);
}

static t_lsttoken	*lexer_lstadd(t_lsttoken *token, int input_len)
{
	token->next = malloc(sizeof(t_lsttoken));
	if (token->next == NULL)
		return (NULL);
	token = token->next;
	lexer_lstinit(token, input_len);
	return (token);
}

static t_lsttoken *lexer_lstlast(t_lsttoken *token)
{
	t_lsttoken *return_p;

	return_p = token;
	while (return_p && return_p->next)
	{
		return_p = return_p->next;
	}
	return (return_p);
}

/*
** token->dataに終端文字を入れて、新しいリストをくっつける
*/
static t_lsttoken	*lexer_evaluate_next(t_lsttoken *token, t_lexer *lexer, int input_len)
{
	token->data[lexer->data_idx] = '\0';
	lexer->data_idx = 0;
	return (lexer_lstadd(token, input_len));//mallocで失敗した時return(NULL)くる
}

static t_lsttoken	*lexer_evaluate_unique(t_lsttoken *token, t_lexer *lexer, char input_c, int input_len)
{
	if  (lexer->data_idx != 0)
	{
		token = lexer_evaluate_next(token, lexer, input_len);
	}
	token->data[0] = input_c;
	token->data[1] = '\0';
	return (lexer_lstadd(token, input_len));
}

static void	lexer_evaluate_dquote(t_lsttoken *token, t_lexer *lexer, char input_c)
{
	if (input_c == CHAR_DQUOTE)
	{
		lexer->state = STATE_GENERAL;
		lexer->dquote = 0;
		return ;
	}
	token->data[lexer->data_idx++] = input_c;
}

static void	lexer_evaluate_squote(t_lsttoken *token, t_lexer *lexer, char input_c)
{
	if (input_c == CHAR_SQUOTE)
	{
		lexer->state = STATE_GENERAL;
		lexer->squote = 0;
		return ;
	}
	token->data[lexer->data_idx++] = input_c;
}

/*
** inputを１文字ずつ評価
*/
static void	lexer_evaluate_input(t_lsttoken *token, t_lexer *lexer, char *input, int input_len)
{
	int	idx =  0;

	token = lexer_lstlast(token);
	while (input[idx] != '\0')
	{
		if (lexer->state == STATE_GENERAL)
		{
			if (input[idx] == '\'')
			{
				lexer->state = STATE_IN_SQUOTE;
				lexer->squote++;
			}
			else if (input[idx] == '\"')/* '$', '\\'以外は文字列リテラルとされる */
			{
				lexer->state = STATE_IN_DQUOTE;
				lexer->dquote++;
>>>>>>> yuka-dir
			}
			else if (input[idx] == '\\')
			{
				idx++;
<<<<<<< HEAD
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
=======
				token->data[lexer->data_idx++] = input[idx];
			}
			else if (input[idx] == ' ' || input[idx] == '\t')
			{
				if (lexer->data_idx != 0)
				{
					token = lexer_evaluate_next(token, lexer, input_len);
				}
			}
			else if (input[idx] == '|' || input[idx] == ';'
					|| input[idx] == '<' || input[idx]  == '>' || input[idx] == '=')
			{
				token = lexer_evaluate_unique(token, lexer, input[idx], input_len);
			}
			else /* それ以外の文字 */
			{
				token->data[lexer->data_idx++] = input[idx];
			}
		}
		else if (lexer->state == STATE_IN_DQUOTE)
		{
			lexer_evaluate_dquote(token, lexer, input[idx]);
		}
		else if (lexer->state == STATE_IN_SQUOTE)
		{
			lexer_evaluate_squote(token, lexer, input[idx]);
		}
		idx++;
	}
	token->data[lexer->data_idx] = '\0';
}

/*
** 2回目以降の入力後、lexer_evaluate_input()が実行される前に構造体tokenのdataを整える
*/
static void	lexer_prepare_next(t_lsttoken *token, t_lexer *lexer, int next_len)
{
	int	data_len;

	token = lexer_lstlast(token);
	token->data[lexer->data_idx++] = '\n';
	data_len = ft_strlen(token->data);
	next_len += data_len;
	char *tmp = ft_strdup(token->data);
	free(token->data);
	token->data = malloc(sizeof(char *) * (next_len + 1));
	if (token->data == NULL)
	{
		free(tmp);
		tmp = NULL;
		return ;
	}
	ft_memcpy(token->data, tmp, data_len);
	free(tmp);
	tmp = NULL;
}

/*
** トークン(意味のある単語)に分ける
*/
t_lsttoken	*msh_lexer(char *input)
{
	t_lsttoken *token;
	t_lexer		lexer;
	size_t	input_len;
	char	*nextinput;
	int		nextinput_len;

	/* inputの長さを求める */
	input_len = ft_strlen(input);
	/* t_lsttoken　mallocして初期化 */
	token = lexer_lstnew(input_len);
	if (token == NULL)//lexer_lstnew内のmallocで失敗した時
	{
		return (NULL);
	}
	lexer_structinit(&lexer); /*msh_lexer.c内で使う構造体の初期化 */

	/* 1回目の入力評価 */
	lexer_evaluate_input(token, &lexer, input, input_len);
	/* single quoteが入力されて、閉じられていない時(2回目以降の入力催促 & 評価) */
	while (lexer.squote != 0 || lexer.dquote != 0)
	{
		nextinput = NULL;
		ft_putstr_fd("> ", 1);
		if (get_next_line(&nextinput) == GNL_ERR)
		{
			return (NULL);
		}
		nextinput_len = ft_strlen(nextinput);
		lexer_prepare_next(token, &lexer, nextinput_len);
		lexer_evaluate_input(token, &lexer, nextinput, nextinput_len);
		free(nextinput);
		nextinput = NULL;
	}
	return (token);
>>>>>>> yuka-dir
}
