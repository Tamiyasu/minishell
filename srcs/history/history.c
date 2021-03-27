/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 23:58:07 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/27 10:54:28 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"
#include <stdio.h>

void	add_new_hist(t_list **h_list)
{
	t_list			*tmp_list;

	tmp_list = malloc(sizeof(t_list));
	tmp_list->content = malloc(sizeof(t_hist));
	((t_hist *)(tmp_list->content))->hist_str = NULL;
	((t_hist *)(tmp_list->content))->tmp_str = NULL;
	ft_lstadd_front(h_list, tmp_list);
}

char	*history_arrange(int *i, int f, t_list *h_list)
{
	*i += f;
	*i = *i < 0 ? 0 : *i;
	*i = *i < ft_lstsize(h_list) - 1 ? *i : ft_lstsize(h_list) - 1;
	return (ft_strdup(history_get(*i, h_list)));
}

char	*history_destroy(t_list **h_list)
{
	ft_lstclear(h_list, free_hist);
	return (NULL);
}

char	*history_make(int *i, char *str, t_list **h_list)
{
	if (*i != 0 )
		history_reset(*i, *h_list);
	history_set(str, 0, *h_list);
	*i = 0;
	if (ft_strlen(str) != 0)
		add_new_hist(h_list);
	return (NULL);
}

char	*history(char *str, int f)
{
	static int		i;
	static t_list	*h_list;
	char			*ret_str;

	if (str == NULL)
		return (history_destroy(&h_list));
	if (h_list == NULL)
		add_new_hist(&h_list);
	history_set(str, i, h_list);
	if (f == 0)
		ret_str = history_make(&i, str, &h_list);
	else
		ret_str = history_arrange(&i, f, h_list);
	return (ret_str);
}
