/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 14:45:39 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/20 13:51:58 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "minishell.h"

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

pid_t   c_pid(pid_t t);
void    sig_handler_c(int signo);
void    sig_handler_p(int signo);
#endif