/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/22 14:36:11 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 22:33:19 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*return_p;

	return_p = malloc(count * size);
	if (return_p)
		ft_bzero(return_p, count * size);
	else
		ft_enomem();
	return ((void *)return_p);
}
