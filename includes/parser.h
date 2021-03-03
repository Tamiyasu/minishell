/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 00:40:42 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/03 14:23:54 by ysaito           ###   ########.fr       */
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
    FT_SEMICOLON_F
};

typedef struct	s_parser_node
{
    struct	s_parser_node *l_node;
    struct	s_parser_node *r_node;
    t_lsttoken *content;
}				t_parser_node;

t_parser_node   *parser(t_lsttoken *token_list);
t_parser_node   *free_tree(t_parser_node **node);
void node_print(t_parser_node *node, int deepness);

#endif
