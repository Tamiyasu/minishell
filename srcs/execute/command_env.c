/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/25 17:11:47 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 02:23:05 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int	command_env(char **env_data)
{
	int	i;
	int j;

	i = 0;
	while (env_data[i] != NULL)
	{
		j = 0;
		while (env_data[i][j] != '\0')
		{
			if (env_data[i][j] == '=')
				break ;
			j++;
		}
		if (env_data[i][j] != '\0')
			ft_putendl_fd(env_data[i], 1);
		i++;
	}
	return (EXIT_SUCCESS);
}
