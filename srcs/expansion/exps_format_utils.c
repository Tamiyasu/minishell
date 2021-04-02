/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exps_format_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 21:00:28 by ysaito            #+#    #+#             */
/*   Updated: 2021/04/01 21:02:24 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int		is_space(char c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}

void	exps_free(t_data *data, char *tmp_token_data)
{
	free(tmp_token_data);
	tmp_token_data = NULL;
	free(data);
	data = NULL;
}

void	set_token(t_token *token)
{
	free(token->data);
	token->data = NULL;
	token->flag = FT_COMMAND_F;
}
