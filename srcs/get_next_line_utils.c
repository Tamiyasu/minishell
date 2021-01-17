/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:57:55 by ysaito            #+#    #+#             */
/*   Updated: 2020/12/16 11:58:22 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int				ft_strlen(char *str)
{
	int			len;

	len = 0;
	while (str[len] != '\0')
	{
		len++;
	}
	return (len);
}

char			*ft_strdup(char *str)
{
	char		*newstr;
	int			strlen;
	int			i;

	if (str == NULL)
		return (NULL);
	strlen = ft_strlen(str);
	newstr = (char *)malloc(sizeof(char) * (strlen + 1));
	i = 0;
	if (newstr == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		newstr[i] = str[i];
		i++;
	}
	newstr[i] = '\0';
	return (newstr);
}

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

int				ft_strchr(char *str, char c)
{
	int			i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char			*ft_substr(char *str, int start, int end)
{
	char		*newstr;
	int			strlen;

	if (str == NULL)
		return (NULL);
	strlen = ft_strlen(str);
	if (start > strlen || end <= 0)
		return (ft_strdup(""));
	newstr = (char *)malloc(sizeof(char) * (end + 1));
	if (newstr == NULL)
		return (NULL);
	while (start < end)
	{
		newstr[start] = str[start];
		start++;
	}
	newstr[start] = '\0';
	return (newstr);
}
