/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:36 by ysaito            #+#    #+#             */
/*   Updated: 2021/03/27 20:04:24 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMINAL_H
# define TERMINAL_H

# define TERM_READ 1
# define TERM_EOF 0
# define TERM_ERR -1
# define EOT '\4'
# define ESCAPE '\33'
# define BACKSPACE '\177'
# define TERM_CLEAR "\e[2K\e[G"
# define TERM_LEFT "\e[1D"
# define TERM_RIGHT "\e[1C"
# define TERM_BACKSPACE "\b\e[K"

# include "minishell.h"
# include "history.h"
# include "signal_handler.h"
# include "libft.h"

void	init_buf(char **buf_join, int *buf_len, int *cursor_len);
int		terminal(char **line);
char	*term_arrow(char *buf_join, int *buf_len, int *cursor_len);

#endif
