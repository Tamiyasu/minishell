/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:59:25 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/19 17:37:50 by ysaito           ###   ########.fr       */
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
void	lexer_increment(t_lexer *lexer);
t_token	*lexer_check_tokenlen(t_token *token_list, t_lexer *lexer, char *input);
t_token	*lexer_set_token(t_token *token_list, t_lexer *lexer, char *content);
t_token	*lexer_set_char(t_token *token_list, t_lexer *lexer, char *input);
t_token	*lexer_set_redirect(t_token *token_list, t_lexer *lexer, char *input);
void	lexer_squote_count_tokenlen(t_lexer *lexer, char *input);
void	lexer_dquote_count_tokenlen(t_lexer *lexer, char *input);
t_token	*lexer_skip_space(t_token *token_list, t_lexer *lexer, char *input);
t_token	*token_list_new(void	*content);
t_token	*token_list_last(t_token *token_list);
void	token_list_addback(t_token **token_list, t_token *new);
void	free_lst(t_token **token);
void	print_token(t_token *token, char *header);
#endif