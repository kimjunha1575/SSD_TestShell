NAME	=	testshell

SRCS	=	testshell.c	\

OBJS	=	${SRCS:.c=.o}

CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror

REMOVE	=	rm -f

all:		${NAME}

%.o : %.c
			${CC} ${CFLAGS} -c $< -o $@

${NAME}:	${OBJS}
			${CC} ${OBJS} -o ${NAME}

clean:
			${REMOVE} $(OBJS)

fclean:
			@${MAKE} clean
			${REMOVE} ${NAME}

re:
			@${MAKE} fclean
			@${MAKE}

.PHONY:		all clean fclean re