# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/26 20:34:32 by tmurakam          #+#    #+#              #
#    Updated: 2020/12/20 21:14:54 by tmurakam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
SRCS_DIR = srcs
SRCS = ${SRCS_DIR}/main.c
SRCS += ${SRCS_DIR}/utils.c

LIBFT_DIR = libft
LIBFT = ${LIBFT_DIR}/libft.a

INCLUDE = -I./includes -I./${LIBFT_DIR}

LIBS  = -lbsd -L${LIBFT_DIR} -lft

OBJS = ${SRCS:.c=.o}
all: ${NAME}
bonus: ${NAME}

${NAME}: ${OBJS} ${LIBFT}
	${CC} ${CFLAGS} ${INCLUDE} ${OBJS} ${LIBS} -o ${NAME}

${LIBFT}: ${LIBFT_DIR}/*.c
	${MAKE} -C ${LIBFT_DIR}

.c.o:
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	${RM} ${OBJS}
	${MAKE} -C ${LIBFT_DIR}	clean

fclean: clean
	${RM} ${NAME}
	${MAKE} -C ${LIBFT_DIR} fclean

re:	fclean all

.PHONY: all clean fclean re bonus
