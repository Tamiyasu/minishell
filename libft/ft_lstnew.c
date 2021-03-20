/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 17:09:59 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 22:38:45 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*return_p;

	return_p = malloc(sizeof(t_list));
	if (return_p)
	{
		return_p->next = 0;
		return_p->content = content;
	}
	else
		ft_enomem();
	return (return_p);
}
