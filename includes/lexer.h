/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:59:25 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/02 16:11:22 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef struct	s_lexer
{
	int			data_idx;
	int			state;
}				t_lexer;

typedef struct				s_lexer_token
{
	char					*data;
	int						flag;
	struct s_lexer_token	*next;
}							t_lexer_token;


enum
{
	STATE_GENERAL,
	STATE_IN_SQUOTE,
	STATE_IN_DQUOTE,
	STATE_IN_ESCAPESEQ,
};

t_lexer_token	*lexer(char *input);
void		lexer_lstinit(t_lexer_token *token, int input_len);
t_lexer_token	*lexer_lstnew(int input_len);
t_lexer_token	*lexer_lstadd(t_lexer_token *token, int input_len);
t_lexer_token	*lexer_lstlast(t_lexer_token *token);
void		free_lst(t_lexer_token **token);
t_lexer_token	*lexer_setchr_and_next(t_lexer_token *token, t_lexer *lexer, char input_c, int input_len);
t_lexer_token	*lexer_set_redirect(t_lexer_token *token, t_lexer *lexer, char *input, int *idx,  int input_len);
t_lexer_token	*lexer_in_dquote(t_lexer_token *token, t_lexer *lexer, char input_c);
t_lexer_token	*lexer_in_squote(t_lexer_token *token, t_lexer *lexer, char input_c);
t_lexer_token	*lexer_set_quote(t_lexer_token *token, t_lexer *lexer, char input_c);



void	token_list_addback(t_lexer_token **token_list, t_lexer_token *new);
t_lexer_token	*token_list_new(void	*content);
t_lexer_token	*token_list_last(t_lexer_token *token_list);

#endif