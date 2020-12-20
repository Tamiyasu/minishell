# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/26 20:34:32 by tmurakam          #+#    #+#              #
#    Updated: 2020/11/22 10:24:05 by tmurakam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CC = gcc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
SRCS_DIR = srcs
SRCS = ${SRCS_DIR}/main.c
SRCS += ${SRCS_DIR}/get_next_line.c
SRCS += ${SRCS_DIR}/cub3d_bmpwrite.c
SRCS += ${SRCS_DIR}/cub3d_mapcheck.c
SRCS += ${SRCS_DIR}/cub3d_mapeval.c
SRCS += ${SRCS_DIR}/cub3d_initialize.c
SRCS += ${SRCS_DIR}/cub3d_initialize_utils.c
SRCS += ${SRCS_DIR}/cub3d_makeimage.c
SRCS += ${SRCS_DIR}/cub3d_makeimage_utils.c
SRCS += ${SRCS_DIR}/cub3d_makeimage_sprite.c
SRCS += ${SRCS_DIR}/utils.c
SRCS += ${SRCS_DIR}/cub3d_utils1.c
SRCS += ${SRCS_DIR}/cub3d_utils2.c

LIBFT_DIR = libft
LIBFT = ${LIBFT_DIR}/libft.a

MINILIBX_DIR = minilibx-linux
MINILIBX = ${MINILIBX_DIR}/libmlx.a

INCLUDE = -I./includes -I./${LIBFT_DIR} -I./${MINILIBX_DIR}

LIBS  = -lmlx -lX11 -lXext -lm -lbsd -L${LIBFT_DIR} -lft -L${MINILIBX_DIR}

OBJS = ${SRCS:.c=.o}
all: ${NAME}
bonus: ${NAME}

${NAME}: ${OBJS} ${LIBFT} ${MINILIBX}
	${CC} ${CFLAGS} ${INCLUDE} ${OBJS} ${LIBS} -o ${NAME}

${LIBFT}: ${LIBFT_DIR}/*.c
	${MAKE} -C ${LIBFT_DIR}

${MINILIBX}:
	${MAKE} -C ${MINILIBX_DIR}

.c.o:
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	${RM} ${OBJS}
	${MAKE} -C ${MINILIBX_DIR} clean 
	${MAKE} -C ${LIBFT_DIR}	clean 

fclean: clean
	${RM} ${NAME}
	${MAKE} -C ${LIBFT_DIR} fclean	

re:	fclean all

.PHONY: all clean fclean re bonus
