/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/22 14:45:39 by tmurakam          #+#    #+#             */
/*   Updated: 2020/06/24 02:43:45 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char *i_p;

	i_p = (char *)s + ft_strlen(s);
	while (i_p != s && *i_p != c)
		i_p--;
	if (*i_p != c)
		i_p = 0;
	return (i_p);
}
