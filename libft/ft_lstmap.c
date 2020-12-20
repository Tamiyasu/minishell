/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/27 19:09:53 by tmurakam          #+#    #+#             */
/*   Updated: 2020/06/27 19:44:17 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *tmp_p;
	t_list *first_p;

	first_p = 0;
	while (lst)
	{
		tmp_p = ft_lstnew(f(lst->content));
		if (!tmp_p)
		{
			ft_lstclear(&first_p, del);
			break ;
		}
		if (first_p)
			ft_lstadd_back(&first_p, tmp_p);
		else
			first_p = tmp_p;
		lst = lst->next;
	}
	return (first_p);
}
