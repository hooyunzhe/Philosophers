NAME	= philo

SDIR	= src
ODIR	= obj
LDIR	= libft
IDIR	= inc

SRCS_P	= philosophers.c philo_functions.c utils.c

OBJS_P	= ${addprefix ${ODIR}/, ${SRCS_P:.c=.o}}

GCC		= gcc
RM		= rm -f

LIBFT	= libft.a

CFLAGS	= -Wall -Wextra -Werror

all:	${NAME}

${ODIR}/%.o:	${SDIR}/%.c
	${GCC} -I${IDIR} -I${LDIR} ${CFLAGS} -c $< -o $@

${LIBFT}:
	make -C ${LDIR}

${NAME}:	${LIBFT} ${OBJS_P}
	#${GCC} -o ${NAME} ${OBJS_P} -g -fsanitize=address
	${GCC} -o ${NAME} ${OBJS_P} ${LDIR}/${LIBFT}

clean:
	${RM} ${OBJS_P}
	make clean -C ${LDIR}

fclean:
	${RM} ${NAME}
	${RM} ${OBJS_P}
	make fclean -C ${LDIR}

re:	fclean all

.PHONY:	all clean fclean re