/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/08 16:59:25 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 15:07:22 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef struct	s_token
{
	char			*data;
	int				flag;
	struct s_token	*next;
}				t_token;

typedef struct	s_data
{
	int	length;
	int	start;
	int	idx;
}				t_data;

int				lexer(char *input, t_token **token_list_p);
void			data_increment(t_data *lexer);
t_token			*lexer_check_len(t_token *token, t_data *lexer, char *input);
t_token			*lexer_set_token(t_token *token, t_data *lexer, char *content);
t_token			*lexer_set_char(t_token *token, t_data *lexer, char *input);
t_token			*lexer_set_redirect(t_token *token, t_data *lexer, char *input);
int				lexer_count_quote(t_data *lexer, char *input, char quote);
int				error_quote(t_token *token_list, char *input, t_data *data);
t_token			*lexer_skip_space(t_token *token, t_data *lexer, char *input);
t_token			*token_list_new(void *content);
t_token			*token_list_last(t_token *token);
void			token_list_addback(t_token **token, t_token *new);
void			free_lst(t_token **token);
#endif
