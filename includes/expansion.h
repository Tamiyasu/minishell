/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 10:13:02 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/29 20:52:50 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"
# include "parser.h"

void	expansion(t_parser_node *node, t_env *env);
t_token	*exps_format(t_token *token);
char	*exps_set_envdata(t_token *token, char *new_data,
						t_data *data, t_env *env);
char	*exps_set_squote_data_1(char *token_data, char *new_data, t_data *data);
char	*exps_set_squote_data_2(char *token_data, char *new_data, t_data *data);
char	*exps_set_dquote_data_1(t_token *token, char *new_data,
						t_data *data, t_env *env);
char	*exps_set_dquote_data_2(t_token *token, char *new_data, t_data *data);
char	*save_reading_data(char *token_data, char *new_data, t_data *data);
void	print_token(t_token *token);//del
#endif
