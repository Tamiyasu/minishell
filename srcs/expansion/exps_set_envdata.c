/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exps_set_envdata.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 19:49:42 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 01:52:38 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char
	*replace_variables_with_values(char *new_data, char *env_data)
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

char
	*save_env_data(t_env *env, char *new_data, char *token_data, t_data *data)
{
	char	*env_key;
	char	*env_value;
	int		env_idx;

	if ((ft_isalpha(token_data[data->idx]) || token_data[data->idx] == '_')
			&& token_data[data->idx])
	{
		data_increment(data);
		while ((ft_isalnum(token_data[data->idx]) || token_data[data->idx] == '_')
			&& token_data[data->idx])
			data_increment(data);
	}
	env_key = ft_substr(token_data, data->start, data->length);
	data->length = 0;
	env_idx = env_search(env->data, env_key);
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
	return (new_data);
}

char
	*exps_set_envdata(t_token *token, char *new_data, t_data *data, t_env *env)
{
	char *exit_status_str;

	new_data = save_reading_data(token->data, new_data, data);
	if (token->data[data->idx] == '?')
	{
		exit_status_str = ft_itoa(g_exit_status);
		new_data = replace_variables_with_values(new_data, exit_status_str);
		free(exit_status_str);
		data->start = ++data->idx;
	}
	else
		new_data = save_env_data(env, new_data, token->data, data);
	token->flag = FT_EXPANSION_ENV;
	return (new_data);
}
