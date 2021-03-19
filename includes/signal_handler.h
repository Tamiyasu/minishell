/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 14:45:39 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/18 15:30:59 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "minishell.h"

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

pid_t c_pid(pid_t t);
void sig_handler_c(int signo);
void sig_handler_p(int signo);

#endif