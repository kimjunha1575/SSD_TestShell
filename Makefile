NAME	=	testshell

SRCSPATH= 	srcs
SRCS	=	main.c	\
			shell.c	\

CFILES	=	${addprefix ${SRCSPATH}/, ${SRCS}}
OBJS	=	${CFILES:.c=.o}

INC		=	includes

CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror

REMOVE	=	rm -f

all:		${NAME}

%.o : %.c
			${CC} ${CFLAGS} -c $< -o $@ -I ${INC}

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