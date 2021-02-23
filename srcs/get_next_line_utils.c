/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:57:55 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/24 00:32:53 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char			*ft_strjoin_and_free(char *s1, char *s2)
{
	char		*newstr;
	int			strlen;
	int			i;
	int			j;
	if (s1 == NULL || s2 == NULL)
	return (NULL);
	strlen = ft_strlen(s1) + ft_strlen(s2);
	newstr = (char *)malloc(sizeof(char) * (strlen + 1));
	if (newstr == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		newstr[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		newstr[i] = s2[j];
		i++;
		j++;
	}
	newstr[i] = '\0';
	free(s1);
	return (newstr);
}
