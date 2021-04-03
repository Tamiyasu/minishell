# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/16 15:27:22 by ysaito            #+#    #+#              #
#    Updated: 2021/04/03 21:48:02 by tmurakam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
SRC_DIR		= ./srcs
SRC_FILE	= main.c
SRC_FILE	+= minishell_utils.c
SRC_FILE	+= env_set_data.c
SRC_FILE	+= env_utils.c
SRC_FILE	+= signal_handler.c
SRC_FILE	+= terminal/terminal.c
SRC_FILE	+= terminal/terminal_utils.c
SRC_FILE	+= terminal/term_bufutils.c
SRC_FILE	+= lexer/lexer.c
SRC_FILE	+= lexer/lexer_lst.c
SRC_FILE	+= lexer/lexer_count_quote.c
SRC_FILE	+= lexer/lexer_set_redirect.c
SRC_FILE	+= lexer/lexer_set_char.c
SRC_FILE	+= lexer/lexer_skip_space.c
SRC_FILE    += parser/parser.c
SRC_FILE    += parser/parser_check_type.c
SRC_FILE    += parser/parser_find_node.c
SRC_FILE    += parser/node.c
SRC_FILE	+= expansion/expansion.c
SRC_FILE	+= expansion/exps_format.c
SRC_FILE	+= expansion/exps_format_utils.c
SRC_FILE	+= expansion/exps_set_envdata.c
SRC_FILE	+= expansion/exps_quote.c
SRC_FILE	+= expansion/exps_increment.c
SRC_FILE	+= execute/execute.c
SRC_FILE	+= execute/execute_pipe.c
SRC_FILE	+= execute/execute_utils.c
SRC_FILE	+= execute/exec_fd.c
SRC_FILE	+= execute/exec_redirect.c
SRC_FILE	+= execute/exec_redirect_save.c
SRC_FILE	+= execute/command_cd.c
SRC_FILE	+= execute/command_echo.c
SRC_FILE	+= execute/command_env.c
SRC_FILE	+= execute/command_execve.c
SRC_FILE	+= execute/command_exit.c
SRC_FILE	+= execute/command_export.c
SRC_FILE	+= execute/command_pwd.c
SRC_FILE	+= execute/command_unset.c
SRC_FILE	+= execute/export_format_variable.c
SRC_FILE	+= execute/export_check_dupl.c
SRC_FILE	+= execute/export_cmp_args.c
SRC_FILE	+= execute/export_sort.c
SRC_FILE	+= execute/export_shape_putenv.c
SRC_FILE	+= execute/export_putenv.c
SRC_FILE	+= execute/unset_save_pwd.c
SRC_FILE	+= execute/execve_search_cmdpath.c
SRC_FILE	+= execute/exec_split_env.c
SRC_FILE	+= execute/error.c
SRC_FILE	+= history/history.c
SRC_FILE	+= history/history_utils.c


INC_DIR	= ./includes
SRCS	= $(addprefix $(SRC_DIR)/,$(SRC_FILE))
OBJS	= $(SRCS:.c=.o)
CC		= gcc
RM		= rm -f

LIBS_NAME	=	libft.a
LIBS_DIR	=	./libft

CFLAGS	=	-Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBS_DIR)
CFLAGS	=	-Wall -Wextra -Werror -g -fsanitize=address -I$(INC_DIR) -I$(LIBS_DIR)


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
