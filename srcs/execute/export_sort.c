/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 01:36:33 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/21 01:57:55 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

t_sort	*sort_init(int num)
{
	t_sort	*sort_p;

	sort_p = malloc(sizeof(t_sort));
	if (sort_p == NULL)
		ft_enomem();
	sort_p->loop_count = num;
	return (sort_p);
}

void	sort_set(t_sort *sort, int num)
{
	sort->left = (num - 2);
	sort->right = (num - 1);
	sort->count = 1;
}

void	sort_updata(t_sort *sort)
{
	sort->count++;
	sort->left--;
	sort->right--;
}

void	export_sort(char **envp, int *idx, int num)
{
	t_sort	*sort;
	int		i;
	int		tmp;

	sort = sort_init(num);
	i = 0;
	while (sort->loop_count)
	{
		sort_set(sort, num);
		while (sort->count < sort->loop_count)
		{
			i = 0;
			while (envp[idx[sort->left]][i] == envp[idx[sort->right]][i])
				i++;
			if (envp[idx[sort->left]][i] > envp[idx[sort->right]][i])
			{
				tmp = idx[sort->left];
				idx[sort->left] = idx[sort->right];
				idx[sort->right] = tmp;
			}
			sort_updata(sort);
		}
		sort->loop_count--;
	}
	free(sort);
}
