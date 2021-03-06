/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 00:56:58 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/05 21:23:59 by tmurakam         ###   ########.fr       */
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
        printf("it ________________________new!");
        *token = new;
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
    {
        printf("##################### %s\n", (*node)->content->data);
        printf("--------------------------\n");
        free_lst(&((*node)->content));
        free_tree(&((*node)->l_node));
        free_tree(&((*node)->r_node));
    }
    free(*node);
    *node = NULL;
    return (*node);
}

void indent(int i)
{
    while(i--)
        printf("  ");
}

void    node_print(t_parser_node *node, int deepness)
{
    indent(deepness);
    printf("pointer : %p\n", node);
    if(node)
    {
        indent(deepness);
        print_token(node->content, "token_list : ");
        indent(deepness);
        printf("r_node : %p\n", node->r_node);
        node_print(node->r_node, deepness + 1);
        indent(deepness);
        printf("l_node : %p\n", node->l_node);
        node_print(node->l_node, deepness + 1);
    }
}

int is_redirect(int flag)
{
    return (
        flag == FT_REDIRECT_O_F ||
        flag == FT_REDIRECT_A_F ||
        flag == FT_REDIRECT_I_F 
    );
}

int check_token_type(t_lsttoken *token, int last_type)
{
    int type_i;
    int token_length;

    token_length = ft_strlen(token->data);
    if(token->data[token_length - 1] == '>')
        {
            if(token_length == 1 || token->data[token_length - 2] != '>')
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
    else if(is_redirect(last_type))
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
        if(node->r_node)
        {
            ret_node = find_command_node(node->r_node);    
        }
        else
        {
            node->r_node = malloc(sizeof(t_parser_node));
            node->r_node->content = NULL;
            node->r_node->r_node = NULL;
            node->r_node->l_node = NULL;
            ret_node = node->r_node;
        }
    }
    else if (is_redirect(node->content->flag))
    {
        while(node->content->flag != FT_COMMAND_F)
        {
            node = node->l_node;
        }
        ret_node = node;
    }
    return (ret_node);
}

t_parser_node   *find_parent_node(t_parser_node   *node)
{
    t_parser_node *ret_node;
    ret_node = NULL;

    if  (node->content->flag == FT_PIPE_F || node->content->flag == FT_SEMICOLON_F)
    {
        if(node->r_node)
        {
            ret_node = find_parent_node(node->r_node);    
        }
        if(!ret_node)
        {
            ret_node = node;
        }
    }
    return (ret_node);
}

t_parser_node   *find_redirect_node(t_parser_node   *node)
{
    t_parser_node *ret_node;
    ret_node = NULL;
    if (node && is_redirect(node->content->flag))
    {
        ret_node = node;
    }
    else if (node && (node->content->flag == FT_PIPE_F || node->content->flag == FT_SEMICOLON_F))
    {
        if(node->r_node)
        {
            ret_node = find_redirect_node(node->r_node);    
        }
    }
    return (ret_node);
}

t_parser_node   *parser(t_lsttoken *token_list)
{
    t_parser_node *node;
    t_lsttoken *token;
    t_lsttoken *next;
    t_parser_node *command_node;
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

        c_type = check_token_type(token, last_type);
        printf("last_type : %d\n", c_type);
        token->flag = c_type;
        if(c_type == FT_COMMAND_F)
        {
            printf("token: %s\n", token->data);
            command_node = find_command_node(node);
            printf("node: %p\n", node);
            printf("command_node: %p\n", command_node);
            command_node->content = lexer_lstadd_back(&command_node->content, token);
        }
        else if(c_type == FT_PIPE_F || c_type == FT_SEMICOLON_F){
            t_parser_node *new_node = malloc(sizeof(t_parser_node));
            new_node->l_node = node;
            new_node->content = token;
            new_node->r_node = NULL;
            node = new_node;
        }
        else if(is_redirect(c_type))
        {
            t_parser_node *new_node = malloc(sizeof(t_parser_node));
            new_node->content = token;
            new_node->r_node = NULL;
            command_node = find_parent_node(node);
            if(command_node)
            {
                new_node->l_node = command_node->r_node;
                command_node->r_node = new_node;
            }
            else
            {
                new_node->l_node = node;
                node = new_node;
            }
        }
        else if(c_type == FT_FILENAME_F)
        {
            command_node = find_redirect_node(node);
            command_node->r_node = malloc(sizeof(t_parser_node));
            command_node->r_node->content = token;
            command_node->r_node->l_node = NULL;
            command_node->r_node->r_node = NULL;
        }
        token = next;
        last_type = c_type;
    } 
    return (node);
}
