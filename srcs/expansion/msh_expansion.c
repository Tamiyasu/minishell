/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/22 10:12:39 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/22 21:31:23 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"
#include "libft.h"

void	msh_expansion(t_lsttoken *token, /*t_env *env, */int *exit_status)
{
	while (token != NULL)
	{
		if (ft_strcmp(token->data, "$?") == 0)
		{
			free(token->data);
			token->data = ft_strdup(ft_itoa(*exit_status));
		}
		token = token->next;
	}
}
