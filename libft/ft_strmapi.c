/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 13:33:26 by tmurakam          #+#    #+#             */
/*   Updated: 2020/06/28 17:53:30 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*return_s;
	size_t			s_len;
	unsigned int	i;

	s_len = ft_strlen(s);
	return_s = ft_calloc(s_len + 1, sizeof(char));
	if (return_s)
	{
		i = 0;
		while (i < s_len)
		{
			*(return_s + i) = f(i, *(s + i));
			i++;
		}
	}
	return (return_s);
}
