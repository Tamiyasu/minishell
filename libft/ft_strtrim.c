/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 01:22:46 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/23 19:47:01 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strtrim(char const *s1, char const *set)
{
	size_t	s1_len;
	size_t	s;
	size_t	e;
	char	*return_s;

	s1_len = ft_strlen(s1);
	s = 0;
	while (*(s1 + s) && ft_strchr(set, *(s1 + s)))
		s++;
	e = s1_len;
	while (0 < e && ft_strchr(set, *(s1 + e - 1)))
		e--;
	if (s == s1_len)
		s = 0;
	return_s = ft_calloc((e - s) + 1, sizeof(char));
	if (return_s && e)
		ft_memcpy(return_s, s1 + s, (e - s));
	return (return_s);
}
