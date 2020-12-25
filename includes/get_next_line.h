/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/16 11:51:36 by ysaito            #+#    #+#             */
/*   Updated: 2020/12/16 15:46:57 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define GNL_READ 1
# define GNL_EOF 0
# define GNL_ERR -1

# include <stdlib.h>
# include <unistd.h>

int		get_next_line(char **line);
// int		ft_strchr(char *str, char c);
// char	*ft_strdup(char *str);
// char	*ft_strjoin_and_free(char *s1, char *s2);
// int		ft_strlen(char *str);
// char	*ft_substr(char *str, int start, int end);

#endif
