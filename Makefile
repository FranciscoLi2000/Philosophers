# ========= CONFIG ============
NAME		= philosophers

CC		= cc
CFLAGS		= -Wall -Wextra -Werror -pthread

SRCS		= ft_putchar_fd.c ft_putstr_fd.c main.c clean_all.c \
		ft_putnbr_fd.c ft_atoi.c init.c monitor.c routine.c

OBJS		= $(SRCS:.c=.o)

INCLUDES	= philosophers.h

# ========= RULES =============
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

# ========= CLEANING ==========
clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

# ========= BONUS (可选) =======
debug:
	$(MAKE) CFLAGS="-g3 -fsanitize=address -Wall -Wextra -Werror" re

# ========= PHONY RULES ========
.PHONY: all clean fclean re
