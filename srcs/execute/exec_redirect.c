/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:40:46 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/30 10:40:22 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int		check_fdnum(char *data, int redirect_flag)
{
	int	fd_num;

	fd_num = ft_atoi(data);
	if (fd_num == 0 && (redirect_flag == FT_REDIRECT_O_F
		|| redirect_flag == FT_REDIRECT_A_F))
		fd_num = 1;
	return (fd_num);
}

int		open_file(char *file, int flag)
{
	int	open_fd;

	if (flag == FT_REDIRECT_I_F)
		open_fd = open(file, O_RDONLY);
	else if (flag == FT_REDIRECT_O_F)
		open_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	else
		open_fd = open(file, O_WRONLY | O_CREAT | O_APPEND,
						S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	return (open_fd);
}

char	*format_fdnum(char *data)
{
	char	*format_data;
	int		i;

	i = 0;
	while (data[i])
	{
		if (!ft_isdigit(data[i]))
			break ;
		i++;
	}
	format_data = ft_substr(data, 0, i);
	free(data);
	return (format_data);
}

void	error_open_fd(t_parser_node *node, int fd_num, int open_fd)
{
	if (open_fd == -1)
		output_error(node->r_node->content->data, strerror(errno));
	else if (fd_num > FD_MAX)
	{
		node->content->data = format_fdnum(node->content->data);
		output_error(node->content->data, "Bad file descriptor");
	}
	else
		output_error("file descriptor out of range", "Bad file descriptor");
}

void	exec_redirect(t_parser_node *node, t_info_fd *msh_fd, t_env *env,
					void (*func)(t_parser_node *node,
					t_env *env, t_info_fd *msh_fd))
{
	int	fd_num;
	int open_fd;

	fd_num = check_fdnum(node->content->data, node->content->flag);
	fprintf(stderr, "data=[%s], fd_num=[%d]\n", node->content->data ,fd_num);//del;
	open_fd = open_file(node->r_node->content->data, node->content->flag);
	if (open_fd == -1 || fd_num > FD_MAX || fd_num < 0)
	{
		error_open_fd(node, fd_num, open_fd);
		g_exit_status = 1;
		return ;
	}
	if (redirect_check_reserve(msh_fd, fd_num, node->content->flag)
		&& node->l_node && node->l_node->content)
	{
		msh_fd = redirect_save_fd(msh_fd, fd_num, node->content->flag);
		dup2(open_fd, fd_num);
	}
	fprintf(stderr, "--------after redirect_save--------------\n");//del;
	fprintf(stderr, "data=[%s], fd_num=[%d], fd_save=[%d]\n", node->content->data , msh_fd->fd_num, msh_fd->fd_save);//del;
	func(node->l_node, env, msh_fd);
}
