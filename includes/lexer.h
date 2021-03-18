/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:59:25 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/18 18:18:47 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef struct				s_token
{
	char					*data;
	int						flag;
	struct s_token			*next;
}							t_token;

typedef struct				s_lexer
{
	int						token_len;
	int						start;
	int						idx;
}							t_lexer;

t_token	*lexer(char *input);
void	free_lst(t_token **token);
void	token_list_addback(t_token **token_list, t_token *new);
t_token	*token_list_new(void	*content);
t_token	*token_list_last(t_token *token_list);
void	print_token(t_token *token, char *header);
#endif