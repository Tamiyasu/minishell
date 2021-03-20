/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 10:12:39 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 00:03:43 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "parser.h"
#include "execute.h"

char	*save_reading_data(char *token_data, char *new_data, t_data *data)
{
	char	*tmp;
	char	*tmp2;

	if (data->length)
	{
		if (new_data == NULL)
			new_data = ft_substr(token_data, data->start, data->length);
		else
		{
			tmp = ft_strdup(new_data);
			tmp2 = ft_substr(token_data, data->start, data->length);
			free(new_data);
			new_data = ft_strjoin(tmp, tmp2);
			free(tmp);
			free(tmp2);
		}
	}
	data->length = 0;
	data->start = ++data->idx;
	return (new_data);
}

void	set_expansion_data(t_token *token, char *new_data)
{
	if (new_data)
	{
		free(token->data);
		token->data = new_data;
	}
	else
	{
		free(token->data);
		token->data = ft_strdup("");
	}
}

void	exps_check_command(t_token *token, t_env *env)
{
	//t_token	*new_token;
	t_data	*data;
	char	*new_data;

	//new_token = NULL;
	while (token)
	{
		data = data_init();
		new_data = NULL;
		while (token->data[data->idx] != '\0')
		{
			if (token->data[data->idx] == '\'')
				new_data = exps_set_squote_data(token->data, new_data, data);
			else if (token->data[data->idx] == '\"')
				new_data = exps_set_dquote_data(token, new_data, data, env);
			else if (token->data[data->idx] == '$')
				new_data = exps_set_envdata(token, new_data, data, env);
			else
				data_increment(data);
		}
		new_data = save_reading_data(token->data, new_data, data);
		set_expansion_data(token, new_data);
		free(data);
		token = token->next;
	}
}

void	print_token(t_token *token)
{
	while (token)
	{
		printf("token->data[%s]\n", token->data);
		token = token->next;
	}
}

void	expansion(t_parser_node *node, t_env *env)
{
	if (!node || !(node->content))
		return ;
	if (node->content->flag == FT_COMMAND_F)
	{
		exps_check_command(node->content, env);
		print_token(node->content);
		node->content = exps_format(node->content);
		printf("-----------------------\n");
		print_token(node->content);
	}
	expansion(node->l_node, env);
	if (node->content->flag == FT_SEMICOLON_F)
		return ;
	expansion(node->r_node, env);
}
