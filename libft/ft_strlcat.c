/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/22 14:42:27 by tmurakam          #+#    #+#             */
/*   Updated: 2020/06/30 11:43:55 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t i;
	size_t src_l;
	size_t dst_l;

	src_l = ft_strlen(src);
	dst_l = ft_strlen(dst);
	if (dst_l >= dstsize)
		return (src_l + dstsize);
	i = 0;
	while (*(src + i) != 0 && (i + dst_l) < (dstsize - 1))
	{
		*(dst + dst_l + i) = *(src + i);
		i++;
	}
	*(dst + dst_l + i) = 0;
	return (src_l + dst_l);
}
