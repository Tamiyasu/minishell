/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 02:08:22 by tmurakam          #+#    #+#             */
/*   Updated: 2020/10/31 14:47:26 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	char	tmp[11];
	char	*return_s;
	int		o;
	int		i;

	o = 0;
	tmp[o++] = '0' + (0 <= n ? n % 10 : -(n % 10));
	while (n / 10)
	{
		n /= 10;
		tmp[o++] = '0' + (0 <= n ? n % 10 : -(n % 10));
	}
	if (n < 0)
		tmp[o++] = '-';
	return_s = ft_calloc(o + 1, sizeof(char));
	if (return_s)
	{
		i = 0;
		while (o)
			*(return_s + i++) = tmp[--o];
	}
	return (return_s);
}
