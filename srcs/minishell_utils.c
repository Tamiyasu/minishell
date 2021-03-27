/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 23:06:58 by tmurakam          #+#    #+#             */
/*   Updated: 2021/03/27 18:07:01 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"
#include "expansion.h"
#include "execute.h"
#include "terminal.h"
#include "parser.h"
#include "signal_handler.h"

void	data_increment(t_data *data)
{
	data->idx++;
	data->length++;
}

t_data	*data_init(void)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if (data)
	{
		data->length = 0;
		data->start = 0;
		data->idx = 0;
	}
	else
		ft_enomem();
	return (data);
}

int		faile_func(int result, char **line)
{
	free(*line);
	*line = NULL;
	if (!result)
	{
		g_exit_status = EXIT_SYNTAX_ERROR;
		ft_putendl_fd(error_str("minishell: "), STDERR_FILENO);
		error_str(NULL);
	}
	return (result);
}

void	free_args(char **args)
{
	int	idx;

	idx = 0;
	if (args == NULL)
	{
		return ;
	}
	while (args[idx] != NULL)
	{
		free(args[idx]);
		idx++;
	}
	free(args);
	args = NULL;
}

char	*error_str(char *str)
{
	static char	*s_str;
	char		*tmp;

	if (str)
	{
		if (s_str)
		{
			tmp = s_str;
			s_str = ft_strjoin(str, s_str);
			free(tmp);
		}
		else
			s_str = ft_strdup(str);
	}
	else
	{
		free(s_str);
		s_str = NULL;
	}
	return (s_str);
}
