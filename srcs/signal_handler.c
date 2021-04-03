/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 14:48:02 by tmurakam          #+#    #+#             */
/*   Updated: 2021/04/03 19:58:56 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_handler.h"
#include <stdio.h>
#include "libft.h"

pid_t	c_pid(pid_t t)
{
	static pid_t c_pid;

	if (0 <= t)
		c_pid = t;
	return (c_pid);
}

int		last_signal(int i)
{
	static int	memory;
	int			last_signal;

	last_signal = memory;
	memory = i;
	return (last_signal);
}

void	sig_handler_c(int signo)
{
	if (c_pid(-1))
		kill(c_pid(-1), signo);
}

void	sig_handler_p(int signo)
{
	if (!c_pid(-1))
	{
		if (signo == SIGINT)
		{
			last_signal(1);
			g_exit_status = 1;
			write(STDERR_FILENO, "\nminishell>> ", 13);
		}
	}
}
