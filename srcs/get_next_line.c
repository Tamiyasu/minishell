/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:10 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/23 20:30:24 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
static int				ft_strlen(char *str)
{
	int			len;

	len = 0;
	while (str[len] != '\0')
	{
		len++;
	}
	return (len);
}
*/

/*
static char			*ft_strdup(char *str)
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
*/

static char			*ft_strjoin_and_free(char *s1, char *s2)
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

/*
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
*/

/*
static char			*ft_substr(char *str, int start, int end)
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
*/

static int				ft_make_one_line(char *buf_join, char **save, char **line)
{
	int			i;

	i = 0;
	if (buf_join == NULL || save == NULL || line == NULL)
		return (GNL_ERR);
	while (buf_join[i] != '\n' && buf_join[i] != '\0')
	{
		i++;
	}
	if (buf_join[i] == '\n')
	{
		*line = ft_substr(buf_join, 0, i);
		*save = ft_strdup(&buf_join[i + 1]);
		free(buf_join);
		return (GNL_READ);
	}
	*line = ft_strdup(buf_join);
	free(buf_join);
	return (GNL_EOF);
}

static char			*ft_read_or_save(char **save, int *rc)
{
	char		*buf;

	if (save == NULL)
		return (NULL);
	if (*save != NULL)
	{
		buf = *save;
		*save = NULL;
		*rc = GNL_READ;
	}
	else
	{
		buf = (char *)malloc(sizeof(char) * (1024 + 1));
		if (buf == NULL)
			return (NULL);
		*rc = read(0, buf, 1024);
		if (*rc < 0)
		{
			free(buf);
			return (NULL);
		}
		buf[*rc] = '\0';
	}
	return (buf);
}

int				get_next_line(char **line)
{
	static char	*save;
	char		*buf;
	char		*buf_join;
	int			rc;

	if (line == NULL)
		return (GNL_ERR);
	buf_join = NULL;
	while (1)
	{
		buf = ft_read_or_save(&save, &rc);
		if (buf == NULL)
		{
			return (GNL_ERR);
		}
		if (buf_join == NULL)
		{
			buf_join = ft_strdup(buf);
		}
		else
		{
			buf_join = ft_strjoin_and_free(buf_join, buf);
		}
		if (ft_strchr(buf, '\n') || rc == GNL_EOF)
		{
			free(buf);
			break;
		}
		free(buf);
	}
	rc = ft_make_one_line(buf_join, &save, line);
	return (rc);
}
