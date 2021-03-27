/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 23:59:30 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/27 11:01:37 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H
# include "libft.h"

typedef struct	s_hist
{
	char			*hist_str;
	char			*tmp_str;
}				t_hist;

void			free_hist(void *hist);
void			history_reset(int i, t_list *h_list);
void			history_set(char *str, int i, t_list *h_list);
char			*history_get(int i, t_list *h_list);
char			*history(char *str, int f);

#endif
