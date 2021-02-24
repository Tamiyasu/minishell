/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 00:56:58 by tmurakam          #+#    #+#             */
/*   Updated: 2021/02/25 01:04:41 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

enum input_type
{
    FT_EMPTY_F,
    FT_FILENAME_F,
    FT_REDIRECT_F,
    FT_COMMAND_F,
    FT_PIPE_F,
};



int check_token_type(t_lsttoken *token, int last_type)
{
    int type_i;
    int token_length;

    token_length = ft_strlen(token->data);
    if(token->data[token_length - 1] == '>')
        type_i = FT_REDIRECT_F;
    else if(token->data[token_length - 1] == '|')
        type_i = FT_PIPE_F;
    else if(last_type == FT_REDIRECT_F)
        type_i = FT_FILENAME_F;
    else 
        type_i = FT_COMMAND_F;
    return(type_i);
}


t_parser_node   *parser(t_lsttoken *token_list)
{
    t_parser_node *node;
    t_lsttoken *token;
    t_lsttoken *next;
    int last_type;
    int c_type;


    last_type = FT_EMPTY_F;
    c_type = FT_EMPTY_F;

    printf("in parser\n");    
    token = token_list;
    node = malloc(sizeof(t_parser_node));
    while(token)
    {
        next = token->next;
        token->next = NULL;

        printf("token->data : %s, ", token->data);
        printf("token->flag : %d, ", token->flag);
        printf("token->next : %p, ", token->next);

        c_type = check_token_type(token, last_type);
        printf("last_type : %d\n", c_type);
        if(c_type == FT_COMMAND_F)
        {
            if(last_type == FT_COMMAND_F)
            {
                ft_lstadd_back(node->content);
            }
        }
        token = next;
    } 

    node = NULL;
    return (node);
}