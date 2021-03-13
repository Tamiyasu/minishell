/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 14:45:39 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/13 14:54:33 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

pid_t c_pid(pid_t t);
void sig_handler(int signo);

#endif