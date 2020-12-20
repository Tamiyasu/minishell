/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 00:33:03 by tmurakam          #+#    #+#             */
/*   Updated: 2020/06/28 15:48:00 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*return_s;
	size_t	s_len;
	size_t	c_len;

	s_len = ft_strlen(s);
	if (start > s_len)
		start = s_len;
	c_len = (s_len - start < len ? s_len - start : len);
	return_s = malloc(sizeof(char) * (c_len + 1));
	if (return_s)
		ft_strlcpy(return_s, s + start, c_len + 1);
	return (return_s);
}
