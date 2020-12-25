# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/12/16 15:27:22 by ysaito            #+#    #+#              #
#    Updated: 2020/12/25 17:52:49 by ysaito           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	minishell
SRC_DIR	:=	./srcs
SRC_FILE	:=	main.c \
				get_next_line.c \
				msh_cd.c \
				msh_echo.c \
				msh_env.c \
				msh_exit.c \
				msh_pwd.c

INC_DIR	:=	./include
SRCS	:=	$(addprefix $(SRC_DIR)/,$(SRC_FILE))
OBJS	:=	$(SRCS:.c=.o)
CC		:=	gcc
AR		:=	ar rc
RM		:=	rm -f

LIBS_NAME	:=	libft.a
LIBS_DIR	:=	./srcs/libft

CFLAGS	:=	-Wall -Wextra -Werror -I$(INC_DIR) -I$(LIBS_DIR)
#CFLAGS	:=	-Wall -Wextra -Werror -g -fsanitize=address -I$(INC_DIR) -I$(MLX_DIR)

$(NAME):	$(OBJS)
			$(MAKE) -C $(LIBS_DIR)
			cp $(LIBS_DIR)/$(LIBS_NAME) .
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS_NAME)

all:		$(NAME)

bonus:		all

clean:
			$(MAKE) -C $(LIBS_DIR) clean
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(LIBS_NAME)
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all bonus clean fclean re
