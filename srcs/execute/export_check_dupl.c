/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_check_dupl.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 01:01:45 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 01:21:46 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int		check_dupl_token(char *fwd_tokend, char **bwd_tokend, t_token *token)
{
	int	save_flag;
	int	idx;

	save_flag = token->flag;
	idx = 0;
	token = token->next;
	while (bwd_tokend[idx] != NULL)
	{
		if (token->flag != -1)
		{
			if (ft_strcmp(fwd_tokend, bwd_tokend[idx]) == 0)
			{
				if (save_flag <= token->flag)
					return (-1);
			}
		}
		idx++;
		token = token->next;
	}
	return (save_flag);
}

void	export_check_dupl(t_token *token, char **split_tokend)
{
	int	idx;

	idx = 0;
	while (split_tokend[idx + 1] != NULL)
	{
		token->flag = check_dupl_token(split_tokend[idx],
										&split_tokend[idx + 1], token);
		token = token->next;
		idx++;
	}
}
