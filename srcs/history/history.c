/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 23:58:07 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/24 00:29:52 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "history.h"

static void set(char *str, int i, t_list *h_list)
{
    int k;

    k = 0;
    while (++k < i && h_list)
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
    static t_list   *h_list;
    t_list          *tmp_list;
    char            *ret_str;

    if(i)
        set(str, i, h_list);
    if (f == 0)
    {
        i = 0;
        tmp_list = malloc(sizeof(t_list));
        tmp_list->content = ft_strdup(str);
        ft_lstadd_front(&h_list, tmp_list);
        ret_str = NULL;
    }
    else 
        ret_str = ft_strdup(get(i, h_list));
    return (ret_str);
}