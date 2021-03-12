/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 10:12:39 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/12 20:34:28 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "expansion.h"
#include "libft.h"

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

char	*save_reading_data(char *token_data, char *new_data, int *start, int *data_len, int *idx)
{
	char	*tmp;

	if(*data_len)
	{
		if (new_data == NULL)
			new_data = ft_substr(token_data, *start, *data_len);
		else
		{
			tmp = ft_strdup(new_data);
			free(new_data);
			new_data = ft_strjoin(tmp, ft_substr(token_data, *start, *data_len));
			free(tmp);
		}
	}
	*data_len = 0;
	*idx = (*idx + 1);
	*start = *idx;
	return (new_data);
}

char	*save_env_data(t_env *env, char *new_data, char *token_data, int *start, int *data_len, int *idx)
{
	char	*env_key;
	char	*env_value;
	int		env_idx;

	while (token_data[*idx] != '$' && token_data[*idx] != '\''
			&& token_data[*idx] != '\"' && token_data[*idx] != ' '
			&& token_data[*idx] != '\t' && token_data[*idx])
	{
		*data_len = (*data_len + 1);
		*idx = (*idx + 1);
	}
	env_key = ft_substr(token_data, *start, *data_len);
	*data_len = 0;
	env_idx = msh_env_search(env->data, env_key);
	if (env_idx == -1)
	{
		*start = (*start + ft_strlen(env_key));
	}
	else
	{
		env_value = ft_strdup(&env->data[env_idx][ft_strlen(env_key) + 1]);
		new_data = replace_variables_with_values(new_data, env_value);
		*start = (*start + ft_strlen(env_key));
	}
	free(env_key);
	return (new_data);
}

char	*set_environment_data(char *token_data, char *new_data, int *start, int *data_len, int *idx, t_env *env, int exit_status)
{
	new_data = save_reading_data(token_data, new_data, start, data_len, idx);
	if (token_data[*idx] == '?')
	{
		new_data = replace_variables_with_values(new_data, ft_itoa(exit_status));
		*idx = (*idx + 1);
		*start = *idx;
	}
	else
	{
		new_data = save_env_data(env, new_data, token_data, start, data_len, idx);
	}
	return (new_data);
}

char	*set_squote_data(char *token_data, char *new_data, int *start, int *data_len, int *idx)
{
	new_data = save_reading_data(token_data, new_data, start, data_len, idx);
	while (token_data[*idx] !=  '\'')
	{
		*data_len = (*data_len + 1);
		*idx = (*idx + 1);
	}
	new_data = save_reading_data(token_data, new_data, start, data_len, idx);
	return (new_data);
}

char	*set_dquote_data(char *token_data, char *new_data, int *start, int *data_len, int *idx, t_env *env, int exit_status)
{
	new_data = save_reading_data(token_data, new_data, start, data_len, idx);
	while (token_data[*idx] !=  '\"')
	{
		if (token_data[*idx] == '\\' && token_data[*idx + 1] == '\\')
			new_data = save_reading_data(token_data, new_data, start, data_len, idx);
		else if (token_data[*idx] == '\\' && token_data[*idx + 1] == '$')
		{
			new_data = save_reading_data(token_data, new_data, start, data_len, idx);
			*data_len = (*data_len + 1);
			*idx = (*idx + 1);
		}
		else if (token_data[*idx] == '$')
		{
			new_data = set_environment_data(token_data, new_data, start, data_len, idx, env, exit_status);
		}
		else
		{
			*data_len = (*data_len + 1);
			*idx = (*idx + 1);
		}
	}
	new_data = save_reading_data(token_data, new_data, start, data_len, idx);
	return (new_data);
}

char	*set_escape_data(char *token_data, char *new_data, int *start, int *data_len, int *idx)
{
	new_data = save_reading_data(token_data, new_data, start, data_len, idx);
	*data_len = (*data_len + 1);
	*idx = (*idx + 1);
	return (new_data);
}

void	expansion_check(t_lsttoken *token_list, t_env *env, int *exit_status)
{
	char	*new_data;
	int		data_len;
	int		start;
	int		idx;

	print_token(token_list, "check");

	while (token_list != NULL)
	{
		new_data = NULL;
		data_len = 0;
		start = 0;
		idx = 0;

		while (token_list->data[idx] != '\0')
		{
			if (token_list->data[idx] == '\'')
				new_data = set_squote_data(token_list->data, new_data, &start, &data_len, &idx);
			else if (token_list->data[idx] == '\"')
				new_data = set_dquote_data(token_list->data, new_data, &start, &data_len, &idx, env, *exit_status);
			else if (token_list->data[idx] == '$')
				new_data = set_environment_data(token_list->data, new_data, &start, &data_len, &idx, env, *exit_status);
			else if (token_list->data[idx] == '\\' && token_list->data[idx + 1] != ' '
					&& token_list->data[idx] != '\t' && token_list->data[idx + 1])
				new_data = set_escape_data(token_list->data, new_data, &start, &data_len, &idx);
			else
			{
				data_len++;
				idx++;
			}
		}
		new_data = save_reading_data(token_list->data, new_data, &start, &data_len, &idx);
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
		token_list = token_list->next;
	}
}

void	expansion(t_parser_node *node, t_env *env, int *exit_status)
{
	if (node == NULL)
		return ;

	if (node->content->flag == FT_COMMAND_F)
	{
		expansion_check(node->content, env, exit_status);
	}
	expansion(node->l_node, env, exit_status);
	expansion(node->r_node, env, exit_status);
}
