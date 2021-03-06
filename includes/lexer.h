/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:59:25 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/05 21:17:55 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef struct				s_lsttoken
{
	char					*data;
	int						flag;
	struct s_lsttoken		*next;
}							t_lsttoken;

t_lsttoken	*lexer(char *input);
void		free_lst(t_lsttoken **token);
void		token_list_addback(t_lsttoken **token_list, t_lsttoken *new);
t_lsttoken	*token_list_new(void	*content);
t_lsttoken	*token_list_last(t_lsttoken *token_list);
void print_token(t_lsttoken *token, char *header);
#endif