/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:59:25 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/01 15:43:43 by ysaito           ###   ########.fr       */
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

t_lsttoken	*lexer(char *input);
void		lexer_lstinit(t_lsttoken *token, int input_len);
t_lsttoken	*lexer_lstnew(int input_len);
t_lsttoken	*lexer_lstadd(t_lsttoken *token, int input_len);
t_lsttoken	*lexer_lstlast(t_lsttoken *token);
void		free_lst(t_lsttoken **token);
t_lsttoken	*lexer_setchr_and_next(t_lsttoken *token, t_lexer *lexer, char input_c, int input_len);
t_lsttoken	*lexer_set_redirect(t_lsttoken *token, t_lexer *lexer, char *input, int *idx,  int input_len);
t_lsttoken	*lexer_in_dquote(t_lsttoken *token, t_lexer *lexer, char input_c);
t_lsttoken	*lexer_in_squote(t_lsttoken *token, t_lexer *lexer, char input_c);
t_lsttoken	*lexer_set_quote(t_lsttoken *token, t_lexer *lexer, char input_c);


#endif