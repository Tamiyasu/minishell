/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 23:59:30 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/28 21:05:15 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H
# include "libft.h"

# define HISTORY_POS_RESET -100
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
char			*history_pos_reset(int *i, t_list *h_list);

#endif
