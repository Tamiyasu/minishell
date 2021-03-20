# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/16 15:27:22 by ysaito            #+#    #+#              #
#    Updated: 2021/03/20 14:59:51 by ysaito           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
SRC_DIR		= ./srcs
SRC_FILE	= main.c
SRC_FILE	+= get_next_line.c
SRC_FILE	+= env_set_data.c
SRC_FILE	+= env_utils.c
SRC_FILE	+= signal_handler.c
SRC_FILE	+= lexer/lexer.c
SRC_FILE	+= lexer/lexer_lst.c
SRC_FILE	+= lexer/lexer_quote.c
SRC_FILE	+= lexer/lexer_set_redirect.c
SRC_FILE	+= lexer/lexer_set_char.c
SRC_FILE	+= lexer/lexer_skip_space.c
SRC_FILE    += parser/parser.c
SRC_FILE    += parser/parser_check_type.c
SRC_FILE    += parser/parser_find_node.c
SRC_FILE    += parser/node.c
SRC_FILE	+= expansion/expansion.c
SRC_FILE	+= execute/execute.c
SRC_FILE	+= execute/execute_cd.c
SRC_FILE	+= execute/execute_echo.c
SRC_FILE	+= execute/execute_env.c
SRC_FILE	+= execute/command_execve.c
SRC_FILE	+= execute/execute_exit.c
SRC_FILE	+= execute/execute_export.c
SRC_FILE	+= execute/execute_pwd.c
SRC_FILE	+= execute/execute_unset.c
SRC_FILE	+= execute/export_putenv.c
SRC_FILE	+= execute/execute_split_env.c
SRC_FILE	+= execute/error.c


INC_DIR	= ./includes
SRCS	= $(addprefix $(SRC_DIR)/,$(SRC_FILE))
OBJS	= $(SRCS:.c=.o)
CC		= gcc
RM		= rm -f

LIBS_NAME	=	libft.a
LIBS_DIR	=	./libft

#CFLAGS	=	-Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBS_DIR)
CFLAGS	:=	-Wall -Wextra -Werror -g -fsanitize=address -I$(INC_DIR) -I$(LIBS_DIR)

$(NAME):	$(OBJS)
			$(MAKE) -C $(LIBS_DIR)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBS_DIR) -lft

all:		$(NAME)

bonus:		all

clean:
			$(MAKE) -C $(LIBS_DIR) clean
			$(RM) $(OBJS)

fclean:		clean
			$(MAKE) -C $(LIBS_DIR) fclean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all bonus clean fclean re
