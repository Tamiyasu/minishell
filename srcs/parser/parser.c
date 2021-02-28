/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 00:56:58 by tmurakam          #+#    #+#             */
/*   Updated: 2021/02/28 20:21:04 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "libft.h"

t_lsttoken	*lexer_lstadd_back(t_lsttoken **token, t_lsttoken *new)
{
    t_lsttoken *token_i;

    printf("in add_back new : %s\n", new->data);
    if (!(*token))
    {
        return(new);
    }
    printf("in add_back *token : %s\n", (*token)->data);
    token_i = *token;
    while (token_i->next)
    {
        token_i = token_i->next;
    }
	token_i->next = new;
	return (*token);
}

t_parser_node *free_tree(t_parser_node **node)
{
    printf("--- : %p\n", (*node));
    if((*node))
        free_lst(&((*node)->content));
    if((*node) && (*node)->l_node)
        free_tree(&((*node)->l_node));
    if((*node) && (*node)->r_node)
        free_tree(&((*node)->l_node));
    free(*node);
    *node = NULL;
    return (*node);
}

int check_token_type(t_lsttoken *token, int last_type)
{
    int type_i;
    int token_length;

    token_length = ft_strlen(token->data);
    if(token->data[token_length - 1] == '>')
        {
            if(token_length == 1 || token->data[token_length - 1] != '>')
                type_i = FT_REDIRECT_O_F;
            else
                type_i = FT_REDIRECT_A_F;
        }
    else if(token->data[token_length - 1] == '|')
        type_i = FT_PIPE_F;
    else if(token->data[token_length - 1] == ';')
        type_i = FT_SEMICOLON_F;
    else if(token->data[token_length - 1] == '<')
        type_i = FT_REDIRECT_I_F;
    else if(last_type == FT_REDIRECT_O_F || last_type == FT_REDIRECT_A_F || last_type == FT_REDIRECT_I_F)
        type_i = FT_FILENAME_F;
    else 
        type_i = FT_COMMAND_F;
    return(type_i);
}
t_parser_node   *find_command_node(t_parser_node *node)
{
    t_parser_node *ret_node;
    ret_node = NULL;

    if(node->content == NULL || node->content->flag == FT_COMMAND_F)
    {
        ret_node = node;
    }
    else if (node->content->flag == FT_PIPE_F || node->content->flag == FT_SEMICOLON_F)
    {
        node->r_node = malloc(sizeof(t_parser_node));
        node->r_node->content = NULL;
        node->r_node->r_node = NULL;
        node->r_node->l_node = NULL;
        ret_node = node->r_node;
    }
    else if (node->content->flag == FT_REDIRECT_A_F || node->content->flag == FT_REDIRECT_I_F || node->content->flag == FT_REDIRECT_O_F)
    {
        while(node->content->flag != FT_COMMAND_F)
        {
            node = node->l_node;
        }
        ret_node = node;
    }
    return (ret_node);
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
    node->content = NULL;
    node->r_node = NULL;
    node->l_node = NULL;
    while(token)
    {
        next = token->next;
        token->next = NULL;

        printf("token->data : %s, ", token->data);
        printf("token->flag : %d, ", token->flag);
        printf("token->next : %p, ", token->next);

        c_type = check_token_type(token, last_type);
        printf("last_type : %d\n", c_type);
        token->flag = c_type;
        if(c_type == FT_COMMAND_F)
        {
            printf("token: %s\n", token->data);
            t_parser_node *command_node;
            command_node = find_command_node(node);
            printf("node: %p\n", node);
            printf("command_node: %p\n", command_node);
            command_node->content = lexer_lstadd_back(&command_node->content, token);
        }
        else if(c_type == FT_PIPE_F || c_type == FT_REDIRECT_A_F || c_type == FT_REDIRECT_I_F || c_type == FT_REDIRECT_O_F || c_type == FT_SEMICOLON_F){
            t_parser_node *new_node = malloc(sizeof(t_parser_node));
            new_node->l_node = node;
            new_node->content = token;
            new_node->r_node = NULL;
            node = new_node;
        }
        else if(c_type == FT_FILENAME_F)
        {
            node->r_node = malloc(sizeof(t_parser_node));
            node->r_node->content = token;
            node->r_node->l_node = NULL;
            node->r_node->r_node = NULL;
        }
        token = next;
    } 
    return (node);
}