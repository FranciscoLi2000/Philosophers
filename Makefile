NAME		= philosophers

SRC		= \
		clean_all.c ft_putchar_fd.c ft_putstr_fd.c routine.c \
		ft_atoi.c ft_putnbr_fd.c init.c monitor.c \
		main.c

SRCS		= ${addprefix ${PRE}, ${SRC}}

OBJS		= ${SRCS:.c=.o}

PRE		= ./srcs/
HEAD		= ./includes/

RM		= rm -f

CC		= cc
CFLAGS		= -Wall -Wextra -Werror

all: ${NAME}

.c.o:
	${CC} ${CFLAGS} -c -I ${HEAD} $< -o ${<:.c=.o}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
