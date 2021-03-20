/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 18:56:56 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 22:39:09 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		strlen;
	char	*return_p;
	char	*dest;

	strlen = ft_strlen(s1);
	return_p = malloc(sizeof(char) * strlen + 1);
	if (!return_p)
		ft_enomem();
	dest = return_p;
	while (*s1)
		*dest++ = *s1++;
	*dest = 0;
	return (return_p);
}
