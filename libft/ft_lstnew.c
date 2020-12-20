/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 17:09:59 by tmurakam          #+#    #+#             */
/*   Updated: 2020/06/27 17:35:19 by tmurakam         ###   ########.fr       */
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
	return (return_p);
}
