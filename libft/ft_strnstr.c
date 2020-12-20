/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/22 14:43:22 by tmurakam          #+#    #+#             */
/*   Updated: 2020/06/30 11:35:04 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	haystack_len;
	size_t	needle_len;

	haystack_len = ft_strlen(haystack);
	needle_len = ft_strlen(needle);
	i = 0;
	while ((i + needle_len) <= len && (i + needle_len) <= haystack_len)
	{
		j = 0;
		while (j < needle_len)
		{
			if (*(haystack + i + j) != *(needle + j))
				break ;
			j++;
		}
		if (j == needle_len)
			return ((char *)haystack + i);
		i++;
	}
	return (0);
}
