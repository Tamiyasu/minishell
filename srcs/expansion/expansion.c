/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 10:12:39 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 18:30:15 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "parser.h"
#include "execute.h"

char	*replace_variables_with_values(char *new_data, char *env_data)
{
	char *tmp;

	if (new_data == NULL)
		new_data = ft_strdup(env_data);
	else
	{
		tmp = ft_strdup(new_data);
		free(new_data);
		new_data = ft_strjoin(tmp, env_data);
		free(tmp);
	}
	return (new_data);
}

char	*save_reading_data(char *token_data, char *new_data, t_data *data)
{
	char	*tmp;
	char    *tmp2;

	if(data->length)
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

char	*save_env_data(t_env *env, char *new_data, char *token_data, t_data *data)
{
	char	*env_key;
	char	*env_value;
	int		env_idx;

	// while (token_data[data->idx] != '$' && token_data[data->idx] != '\''
	// 		&& token_data[data->idx] != '\"' && token_data[data->idx] != ' '
	// 		&& token_data[data->idx] != '\t' && token_data[data->idx])
	while ((ft_isalpha(token_data[data->idx]) != 0 || token_data[data->idx] == '_')  && token_data[data->idx])
	{
		data_increment(data);
	}
	env_key = ft_substr(token_data, data->start, data->length);
	// printf("env_key=[%s]\n",  env_key);
	data->length = 0;
	env_idx = env_search(env->data, env_key);
	// printf("env_idx=[%d]\n", env_idx);
	if (env_idx == -1)
		data->start += ft_strlen(env_key);
	else
	{
		env_value = ft_strdup(&env->data[env_idx][ft_strlen(env_key) + 1]);
		new_data = replace_variables_with_values(new_data, env_value);
		free(env_value);
		data->start += ft_strlen(env_key);
	}
	free(env_key);
	// printf("new_data[%s]\n", new_data);
	return (new_data);
}

char	*set_environment_data(char *token_data, char *new_data, t_data *data, t_env *env)
{
	char *exit_status_str;

	new_data = save_reading_data(token_data, new_data, data);
	if (token_data[data->idx] == '?')
	{
		exit_status_str = ft_itoa(g_exit_status);
		new_data = replace_variables_with_values(new_data, exit_status_str);
		free(exit_status_str);
		data->start = ++data->idx;
	}
	else
		new_data = save_env_data(env, new_data, token_data, data);
	return (new_data);
}

char	*set_squote_data(char *token_data, char *new_data, t_data *data)
{
	new_data = save_reading_data(token_data, new_data, data);
	while (token_data[data->idx] !=  '\'')
	{
		data_increment(data);
	}
	new_data = save_reading_data(token_data, new_data, data);
	return (new_data);
}

char	*set_dquote_data(char *token_data, char *new_data, t_data *data, t_env *env)
{
	new_data = save_reading_data(token_data, new_data, data);
	while (token_data[data->idx] !=  '\"')
	{
		if (token_data[data->idx] == '$')
		{
			new_data = set_environment_data(token_data, new_data, data, env);
		}
		else
		{
			data_increment(data);
		}
	}
	new_data = save_reading_data(token_data, new_data, data);
	return (new_data);
}

void	set_expansion_data(t_token *token_list, char *new_data)
{
	if (new_data)
	{
		free(token_list->data);
		token_list->data = new_data;
	}
	else
	{
		free(token_list->data);
		token_list->data = ft_strdup("");
	}
}

void	expansion_check(t_token *token_list, t_env *env)
{
	t_token	*new_token_list;
	t_data	*data;
	char	*new_data;

	new_token_list = NULL;
	while (token_list)
	{
		data = data_init();
		new_data = NULL;

		while (token_list->data[data->idx] != '\0')
		{
			if (token_list->data[data->idx] == '\'')
				new_data = set_squote_data(token_list->data, new_data, data);
			else if (token_list->data[data->idx] == '\"')
				new_data = set_dquote_data(token_list->data, new_data, data, env);
			else if (token_list->data[data->idx] == '$')
			{
				new_data = set_environment_data(token_list->data, new_data, data, env);
				token_list->flag = FT_EXPANSION_ENV;
			}
			else
			{
				data_increment(data);
			}
		}
		new_data = save_reading_data(token_list->data, new_data, data);
		set_expansion_data(token_list, new_data);
		free(data);
		token_list = token_list->next;
	}
}

void	print_token(t_token *token)
{
	while (token)
	{
		printf("token->data=[%s][%d]\n", token->data, token->flag);
		token = token->next;
	}
}

t_token	*expansion_format(t_token *token_list)
{
	t_token *token_list_p = token_list;
	t_data *data;
	char	*tmp_token_data;

	while (token_list)
	{
		if (token_list->flag == FT_EXPANSION_ENV && ft_strcmp(token_list->data, "") != 0)//$ECHO="echo -n"
		{
			data = data_init();
			tmp_token_data = ft_strdup(token_list->data);
			free(token_list->data);
			token_list->data = NULL;
			token_list->flag = FT_COMMAND_F;
			while (tmp_token_data[data->idx])
			{
				if (tmp_token_data[data->idx] == ' ' || tmp_token_data[data->idx] == '\t')
				{
					if (data->length)
					{
						if (token_list->data == NULL)
						{
							token_list->data = ft_substr(tmp_token_data, data->start, data->length);
						}
						else
						{
							t_token *add_token_list;

							add_token_list = token_list_new(ft_substr(tmp_token_data, data->start, data->length));
							add_token_list->flag = FT_COMMAND_F;
							add_token_list->next = token_list->next;
							token_list->next = add_token_list;
						}
						data->length = 0;
					}
					data->start = ++data->idx;
				}
				else
				{
					data_increment(data);
				}
			}
			if (data->length)
			{
				if (token_list->data == NULL)
				{
					token_list->data = ft_substr(tmp_token_data, data->start, data->length);
				}
				else
				{
					t_token *add_token_list;

					add_token_list = token_list_new(ft_substr(tmp_token_data, data->start, data->length));
					add_token_list->flag = FT_COMMAND_F;
					add_token_list->next = token_list->next;
					token_list->next = add_token_list;
				}
			}
			free(data);
		}
		token_list = token_list->next;
	}
	return (token_list_p);
}

void	expansion(t_parser_node *node, t_env *env)
{
	if (!node || !(node->content))
		return ;
	if (node->content->flag == FT_COMMAND_F)
	{
		printf("---------------------before expansion check\n");
		print_token(node->content);
		expansion_check(node->content, env);
		printf("---------------------after expansion check\n");
		print_token(node->content);

	 	node->content = expansion_format(node->content);
		printf("---------------------after expansion format\n");
		print_token(node->content);
		printf("---------------------end\n");
	}
	expansion(node->l_node, env);
	if (node->content->flag == FT_SEMICOLON_F)
		return ;
	expansion(node->r_node, env);
}
