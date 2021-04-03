/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 23:59:30 by tmurakam          #+#    #+#             */
/*   Updated: 2021/04/04 07:56:46 by ysaito           ###   ########.fr       */
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

void			add_new_hist(t_list **h_list);
void			free_hist(void *hist);
void			history_reset(int i, t_list *h_list);
void			history_set(char *str, int i, t_list *h_list);
char			*history_get(int i, t_list *h_list);
char			*history(char *str, int f);
char			*history_pos_reset(int *i, t_list *h_list);

#endif
