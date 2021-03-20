/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 21:45:12 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/20 21:59:20 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

t_info_fd	*fd_list_new(int fd_num, int fd_save, int flag)
{
	t_info_fd	*msh_fd;

	msh_fd = malloc(sizeof(t_info_fd));
	if (msh_fd)
	{
		msh_fd->fd_num = fd_num;
		msh_fd->fd_save = fd_save;
		msh_fd->flag = flag;
		msh_fd->next = NULL;
	}
	return (msh_fd);
}

t_info_fd	*fd_list_last(t_info_fd *msh_fd)
{
	t_info_fd	*return_fd;

	return_fd = msh_fd;
	while (return_fd && return_fd->next)
	{
		return_fd = return_fd->next;
	}
	return (return_fd);
}

void		fd_list_addback(t_info_fd **msh_fd, t_info_fd *new)
{
	if (*msh_fd)
		fd_list_last(*msh_fd)->next = new;
	else
		*msh_fd = new;
}

void		free_fd(t_info_fd **msh_fd)
{
	t_info_fd *temp;
	t_info_fd *temp_next;

	temp = *msh_fd;
	while (temp != NULL)
	{
		temp_next = temp->next;
		free(temp);
		temp = temp_next;
	}
	*msh_fd = NULL;
}

void		reset_fd(t_info_fd *msh_fd)
{
	if (!msh_fd)
		return ;
	while (msh_fd)
	{
		dup2(msh_fd->fd_save, msh_fd->fd_num);
		close(msh_fd->fd_save);
		msh_fd = msh_fd->next;
	}
}
