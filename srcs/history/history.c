/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 23:58:07 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/27 10:36:39 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"
#include <stdio.h>

static void print_hist(t_list *h_list)
{
	printf("here - %p\n", h_list);
	while(h_list)
	{
		printf("h_list[%p] : h_list->content[%p], h_list->content->hist_str=[%s], h_list->content->tmp_str=[%s]\n", h_list, h_list->content, ((t_hist *)(h_list->content))->hist_str, ((t_hist *)(h_list->content))->tmp_str);
		h_list = h_list->next;
	}
}

static void reset(int i, t_list *h_list)
{
	int k;

	k = 0;
	while (k++ < i && h_list)
		h_list = h_list->next;
	if (h_list)
	{
		free(((t_hist *)(h_list->content))->tmp_str);
		((t_hist *)(h_list->content))->tmp_str = ft_strdup(((t_hist *)(h_list->content))->hist_str);
	}

}

static void set(char *str, int i, t_list *h_list)
{
	int k;

	printf("in history set : i=[%d] : str=[%s]\n", i, str);
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

static char *get(int i, t_list *h_list)
{
	int k;

	printf("in history get : i=[%d]\n", i);
	k = 0;
	while (k++ < i && h_list->next)
		h_list = h_list->next;
	if (((t_hist *)(h_list->content))->tmp_str)
		return (((t_hist *)(h_list->content))->tmp_str);
	else
		return (((t_hist *)(h_list->content))->hist_str);
}

void    free_hist(void *hist)
{
	free(((t_hist *)hist)->hist_str);
	free(((t_hist *)hist)->tmp_str);
	free(hist);
}

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
	return(ft_strdup(get(*i, h_list)));
}

char	*history_destroy(t_list **h_list)
{
	ft_lstclear(h_list, free_hist);
	return (NULL);
}

char	*history_make(int *i, char *str, t_list **h_list)
{
	if (*i != 0 )
		reset(*i, *h_list);
	set(str, 0, *h_list);
	*i = 0;
	if (ft_strlen(str) != 0)
		add_new_hist(h_list);
	return (NULL);
}

char    *history(char *str, int f)
{
	static int		i;
	static t_list	*h_list;
	char			*ret_str;

	printf("\nin history : str = [%s], f=[%d]\n", str, f);

	if(str == NULL)
		return history_destroy(&h_list);
	if(h_list == NULL)
		add_new_hist(&h_list);
	set(str, i, h_list);
	if (f == 0)
		history_make(&i, str, &h_list);
	else
		ret_str = history_arrange(&i, f, h_list);
	print_hist(h_list);
	return (ret_str);
}
