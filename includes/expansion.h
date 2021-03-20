/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 10:13:02 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 20:30:16 by ysaito           ###   ########.fr       */
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
char	*exps_set_squote_data(char *token_data, char *new_data, t_data *data);
char	*exps_set_dquote_data(t_token *token, char *new_data,
						t_data *data, t_env *env);
char	*save_reading_data(char *token_data, char *new_data, t_data *data);
#endif
