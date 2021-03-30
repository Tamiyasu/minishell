/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect_save.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:50:41 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/30 10:45:29 by ysaito           ###   ########.fr       */
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

int			redirect_check_reserve(t_info_fd *msh_fd, int fd_num, int flag)
{
	while (msh_fd)
	{
		if (flag == FT_REDIRECT_I_F)
		{
			if (msh_fd->flag == FT_REDIRECT_I_F && fd_num == msh_fd->fd_num)
				return (0);
		}
		else
		{
			if ((msh_fd->flag == FT_REDIRECT_O_F || msh_fd->flag == FT_REDIRECT_A_F)
				&& fd_num == msh_fd->fd_num)
				return (0);
		}

		msh_fd = msh_fd->next;
	}
	return (1);
}
