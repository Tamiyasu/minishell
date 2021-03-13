/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 14:48:02 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/13 18:43:40 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_handler.h"
#include <stdio.h>

pid_t c_pid(pid_t t)
{
    static pid_t c_pid;
    if(0 <= t)
        c_pid = t;
    return c_pid;
}

void sig_handler_c(int signo)
{
    if (c_pid(-1))
        kill (c_pid(-1), signo);
}
void sig_handler_p(int signo)
{
    if (signo == SIGINT)
        write(1, "\nminishell>> ", 13);
}