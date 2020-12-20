/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 16:09:42 by tmurakam          #+#    #+#             */
/*   Updated: 2020/06/27 16:33:50 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	if (n / 10)
		ft_putnbr_fd(n / 10, fd);
	else
	{
		if (n < 0)
			ft_putchar_fd('-', fd);
	}
	ft_putchar_fd('0' + (n % 10) * ((0 < n) ? 1 : -1), fd);
}
