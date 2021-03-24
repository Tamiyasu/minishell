/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 23:58:07 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/25 00:17:13 by tmurakam         ###   ########.fr       */
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
        free(h_list->content);
        h_list->content = (void *)ft_strdup(str);
    }
}
static char *get(int i, t_list *h_list)
{
    int k;

    k = 0;
    while (++k < i && h_list->next)
        h_list = h_list->next;
    return (char *)h_list->content;
}

char    *history(char *str, int f)
{
    static int      i;
    static int      s;
    static t_hist   *h_list;
    t_hist          *tmp_list;
    char            *ret_str;

    if(str == NULL)
    {
        ft_lstclear(&h_list, free);
        return (NULL);
    }
    if(s == 0)
    {
        s = 1;
        tmp_list = malloc(sizeof(t_hist));
        tmp_list->hist_str = ft_strdup(str);
        tmp_list->tmp_str = NULL;
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
        i += f;
        i = i < 0 ? 0 : i;
        ret_str = ft_strdup(get(i, h_list));
    return (ret_str);
}