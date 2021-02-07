/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 17:35:44 by ysaito            #+#    #+#             */
/*   Updated: 2021/02/07 20:45:14 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "libft.h"

/*
** lexer->最後にsp" "入力されると空文字が入ってしまう。echoで今対応している状態。
** lexer or paserで対応したら、l.35 && l.47の条件式右側 ft_strcmp~　はいらない。
*/
void	execute_echo(t_lsttoken *token)
{
	token = token->next;
	if (token == NULL)
	{
		ft_putendl_fd("", 1);
		return ;
	}
	if (ft_strcmp(token->data, "-n") == 0)
	{
		token = token->next;
		while (token != NULL)
		{
			ft_putstr_fd(token->data, 1);
			token = token->next;
			if (token ==  NULL || ft_strcmp(token->data, "") == 0)
			{
				break ;
			}
			ft_putchar_fd(' ', 1);
		}
		return ;
	}
	while (token != NULL)
	{
		ft_putstr_fd(token->data, 1);
		token = token->next;
		if (token ==  NULL || ft_strcmp(token->data, "") == 0)
		{
			break ;
		}
		ft_putchar_fd(' ', 1);
	}
	ft_putchar_fd('\n', 1);
	return ;
}
