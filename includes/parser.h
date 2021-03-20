/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 00:40:42 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 13:49:21 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"
# include "lexer.h"
/*
enum	e_input_type
{
	FT_EMPTY_F,
	FT_FILENAME_F,
	FT_REDIRECT_I_F,
	FT_REDIRECT_O_F,
	FT_REDIRECT_A_F,
	FT_COMMAND_F,
	FT_PIPE_F,
	FT_SEMICOLON_F
}; */
# define	FT_EMPTY_F 0
# define	FT_FILENAME_F 1
# define	FT_REDIRECT_I_F 2
# define	FT_REDIRECT_O_F 3
# define	FT_REDIRECT_A_F 4
# define	FT_COMMAND_F 5
# define	FT_PIPE_F 6
# define	FT_SEMICOLON_F 7 

typedef struct	s_parser_node
{
	struct s_parser_node	*l_node;
	struct s_parser_node	*r_node;
	t_token					*content;
}				t_parser_node;

t_parser_node	*free_tree(t_parser_node **node);
t_parser_node	*make_node(t_token *con, t_parser_node *l, t_parser_node *r);
int				parser(t_token *token_list, t_parser_node **node_p);
void			node_print(t_parser_node *node, int deepness);
t_parser_node	*find_c_node(t_parser_node *node);
int				is_redirect(int flag);
int				check_input(int c_type, int last_type);
int				check_last_input(int c_type);
t_parser_node	*find_c_node_p_and_sc(t_parser_node *node);
t_parser_node	*find_c_node(t_parser_node *node);
t_parser_node	*find_parent_node(t_parser_node *node);
t_parser_node	*find_redirect_node(t_parser_node *node);
#endif
