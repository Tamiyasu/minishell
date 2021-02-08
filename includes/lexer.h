/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:59:25 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/08 17:31:09 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef struct	s_lexer
{
	int			data_idx;
	int			squote;
	int			dquote;
	int			state;
}				t_lexer;

t_lsttoken	*msh_lexer(char *input);
void		lexer_lstinit(t_lsttoken *token, int input_len);
t_lsttoken	*lexer_lstnew(int input_len);
t_lsttoken	*lexer_lstadd(t_lsttoken *token, int input_len);
t_lsttoken	*lexer_lstlast(t_lsttoken *token);

#endif