# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/16 15:27:22 by ysaito            #+#    #+#              #
<<<<<<< HEAD
#    Updated: 2021/01/17 21:14:42 by tmurakam         ###   ########.fr        #
=======
#    Updated: 2021/01/26 19:24:37 by ysaito           ###   ########.fr        #
>>>>>>> yuka-dir
#                                                                              #
# **************************************************************************** #

NAME		= minishell
SRC_DIR		= ./srcs
SRC_FILE	= main.c
SRC_FILE	+= get_next_line.c
SRC_FILE	+= msh_cd.c
SRC_FILE	+= msh_echo.c
SRC_FILE	+= msh_env.c
SRC_FILE	+= msh_exit.c
SRC_FILE	+= msh_pwd.c
SRC_FILE	+= msh_lexer.c

INC_DIR	= ./includes
SRCS	= $(addprefix $(SRC_DIR)/,$(SRC_FILE))
OBJS	= $(SRCS:.c=.o)
CC		= gcc
RM		= rm -f

LIBS_NAME	=	libft.a
LIBS_DIR	=	./libft

CFLAGS	=	-Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBS_DIR)
#CFLAGS	:=	-Wall -Wextra -Werror -g -fsanitize=address -I$(INC_DIR) -I$(MLX_DIR)

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
