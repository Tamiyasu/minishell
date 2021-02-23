/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:36 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/24 00:34:25 by tmurakam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define GNL_READ 1
# define GNL_EOF 0
# define GNL_ERR -1

# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

int		get_next_line(char **line);

#endif
