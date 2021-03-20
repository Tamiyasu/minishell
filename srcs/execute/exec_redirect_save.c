/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect_save.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:50:41 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 20:50:52 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

t_info_fd	*redirect_save_fd(t_info_fd *msh_fd, int fd_num, int flag)
{
	t_info_fd *new_fd;

	if (msh_fd == NULL)
	{
		msh_fd = fd_list_new(fd_num, dup(fd_num), flag);
		return (msh_fd);
	}
	new_fd = fd_list_new(fd_num, dup(fd_num), flag);
	fd_list_addback(&msh_fd, new_fd);
	return (msh_fd);
}

int	redirect_check_reserve(t_info_fd *msh_fd, int fd_num, int redirect_flag)
{
	while (msh_fd)
	{
		if (redirect_flag == msh_fd->flag && fd_num == msh_fd->fd_num)
			return (0);
		msh_fd = msh_fd->next;
	}
	return (1);
}
