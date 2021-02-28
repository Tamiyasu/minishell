/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 00:40:42 by tmurakam          #+#    #+#             */
/*   Updated: 2021/02/28 18:10:10 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "lexer.h"

enum input_type
{
    FT_EMPTY_F,
    FT_FILENAME_F,
    FT_REDIRECT_I_F, // <
    FT_REDIRECT_O_F, // >  "254>"
    FT_REDIRECT_A_F, // >> "3>>"
    FT_COMMAND_F,
    FT_PIPE_F,
};

typedef struct	s_parser_node
{
    struct	s_parser_node *l_node;
    struct	s_parser_node *r_node;
    t_lsttoken *content;
}				t_parser_node;

t_parser_node   *parser(t_lsttoken *token_list);

#endif