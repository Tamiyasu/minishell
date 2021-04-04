/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 10:44:47 by tmurakam          #+#    #+#             */
/*   Updated: 2021/04/04 07:56:33 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

void	add_new_hist(t_list **h_list)
{
	t_list			*tmp_list;

	tmp_list = malloc(sizeof(t_list));
	tmp_list->content = malloc(sizeof(t_hist));
	((t_hist *)(tmp_list->content))->hist_str = NULL;
	((t_hist *)(tmp_list->content))->tmp_str = NULL;
	ft_lstadd_front(h_list, tmp_list);
}

void	free_hist(void *hist)
{
	free(((t_hist *)hist)->hist_str);
	free(((t_hist *)hist)->tmp_str);
	free(hist);
}

void	history_reset(int i, t_list *h_list)
{
	int	k;

	k = 0;
	while (k++ < i && h_list)
		h_list = h_list->next;
	if (h_list)
	{
		free(((t_hist *)(h_list->content))->tmp_str);
		((t_hist *)(h_list->content))->tmp_str =
			ft_strdup(((t_hist *)(h_list->content))->hist_str);
	}
}

void	history_set(char *str, int i, t_list *h_list)
{
	int	k;

	k = 0;
	while (k++ < i && h_list)
		h_list = h_list->next;
	if (h_list)
	{
		free(((t_hist *)(h_list->content))->tmp_str);
		((t_hist *)(h_list->content))->tmp_str = ft_strdup(str);
		if (i == 0)
		{
			free(((t_hist *)(h_list->content))->hist_str);
			((t_hist *)(h_list->content))->hist_str = ft_strdup(str);
		}
	}
}

char	*history_get(int i, t_list *h_list)
{
	int	k;

	k = 0;
	while (k++ < i && h_list->next)
		h_list = h_list->next;
	if (((t_hist *)(h_list->content))->tmp_str)
		return (((t_hist *)(h_list->content))->tmp_str);
	else
		return (((t_hist *)(h_list->content))->hist_str);
}
