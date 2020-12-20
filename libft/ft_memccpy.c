/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/22 14:37:55 by tmurakam          #+#    #+#             */
/*   Updated: 2020/11/19 02:20:46 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	size_t i;

	i = 0;
	if (dst != src)
	{
		while (i < n)
		{
			*((unsigned char *)dst + i) = *((unsigned char *)src + i);
			if (*((unsigned char*)src + i++) == (unsigned char)c)
				return (dst + i);
		}
	}
	return (0);
}
