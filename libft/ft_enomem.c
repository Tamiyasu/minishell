/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_enomem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 22:25:59 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 23:13:00 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <errno.h>

void	ft_enomem(void)
{
	ft_putendl_fd(strerror(ENOMEM), 2);
	exit (1);
}
