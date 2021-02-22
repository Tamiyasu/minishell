/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 17:11:47 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/22 10:40:02 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

int	execute_env(char **env_data)
{
	int	idx;

	idx = 0;
	while  (env_data[idx] != NULL)
	{
		int x = 0;
		while (env_data[idx][x] != '\0')
		{
			if (env_data[idx][x] == '=')
				break ;
			x++;
		}
		if (env_data[idx][x] != '\0')
		{
			ft_putendl_fd(env_data[idx], 1);
		}
		idx++;
	}
	return (0);
}
