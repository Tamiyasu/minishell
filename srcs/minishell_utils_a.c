/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils_a.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/04 10:48:29 by tmurakam          #+#    #+#             */
/*   Updated: 2021/04/04 10:51:27 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_set(char **s1, char *s2)
{
	free(*s1);
	if (s2)
		*s1 = ft_strdup(s2);
	else
		*s1 = NULL;
}

int		arr_size(char **null_ended_strs)
{
	int ret;

	ret = 0;
	while (*null_ended_strs++)
		ret++;
	return (ret);
}

char	*strs_join(char **strs, char *enc, char *f)
{
	char *str;
	char *tmp;

	str = ft_strdup(f);
	if (*strs)
	{
		tmp = str;
		str = ft_strjoin(tmp, *(strs++));
		free(tmp);
	}
	while (*strs)
	{
		tmp = str;
		str = ft_strjoin(tmp, enc);
		free(tmp);
		tmp = str;
		str = ft_strjoin(tmp, *(strs++));
		free(tmp);
	}
	return (str);
}

void	join_free(char **s1, char *s2)
{
	char *tmp;

	tmp = *s1;
	*s1 = ft_strjoin(*s1, s2);
	free(tmp);
}
