/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/22 14:29:24 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/19 14:54:07 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\r' || c == '\n' ||
			c == '\t' || c == '\f' || c == '\v');
}

int	ft_atoi(const char *str)
{
	long return_int;
	int input_sign;

	input_sign = 1;
	return_int = 0;
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			input_sign *= -1;
		str++;
	}
	while ('0' <= *str && *str <= '9')
	{
		return_int = return_int * 10 + *(str++) - '0';
		if (return_int > 2147483647)
			return (-1);
	}
	return (input_sign * return_int);
}
