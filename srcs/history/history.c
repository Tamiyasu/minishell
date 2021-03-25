/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 23:58:07 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/25 00:39:42 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

static void set(char *str, int i, t_list *h_list)
{
    int k;

    k = 0;
    while (k++ < i && h_list)
        h_list = h_list->next;
    if (h_list)
    {
        free(((t_hist *)(h_list->content))->tmp_str);
        ((t_hist *)(h_list->content))->tmp_str = ft_strdup(str);
    }
}
static char *get(int i, t_list *h_list)
{
    int k;

    k = 0;
    while (++k < i && h_list->next)
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

char    *history(char *str, int f)
{
    static int      i;
    static int      s;
    static t_list   *h_list;
    t_list          *tmp_list;
    char            *ret_str;

    if(str == NULL)
    {
        ft_lstclear(&h_list, free_hist);
        return (NULL);
    }
    if(s == 0)
    {
        s = 1;
        tmp_list = malloc(sizeof(t_list));
        tmp_list->content = malloc(sizeof(t_hist));
        ((t_hist *)(tmp_list->content))->hist_str = ft_strdup(str);
        ((t_hist *)(tmp_list->content))->tmp_str = NULL;
        ft_lstadd_front(&h_list, tmp_list);
    }
    set(str, i, h_list);
    if (f == 0)
    {
        i = 0;
        s = 0;
        ret_str = NULL;
    }
    else
    {
        i += f;
        i = i < 0 ? 0 : i;
        ret_str = ft_strdup(get(i, h_list));
    }
    return (ret_str);
}