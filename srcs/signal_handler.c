/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/13 14:48:02 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/13 15:14:45 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signal_handler.h"

pid_t c_pid(pid_t t)
{
    static pid_t c_pid;
    if(0 <= t)
        c_pid = t;
    return c_pid;
}

void sig_handler(int signo)
{
    if (c_pid(-1))
    {
        kill (c_pid(-1), signo);
        if (signo == SIGINT)
            write(1, "\n", 1);
        else if (signo == SIGQUIT)
            write(1, "Quit: 3\n", 8);
    }
    else
    {
        if (signo == SIGINT)
            write(1, "\nminishell>>", 12);
        else if (signo == SIGQUIT)
            write(1, "", 0);
    }
}