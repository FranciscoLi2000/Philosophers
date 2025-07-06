# ========= CONFIG ============
NAME		= pipex

CC		= cc
CFLAGS		= -Wall -Wextra -Werror
INCDIRS		= -Iincludes -Ilibft

SRCDIR		= srcs
OBJDIR		= objs
LIBFTDIR	= libft
LIBFT		= $(LIBFTDIR)/libft.a

SRC		= $(wildcard $(SRCDIR)/*.c)
OBJ		= $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# ========= RULES =============
all: $(NAME)

# pipex 的最终链接目标
$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(INCDIRS) $(OBJ) $(LIBFT) -o $(NAME)

# 自动生成 .o 文件到 objs 目录
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

# 编译 libft
$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

# ========= CLEANING ==========
clean:
	$(MAKE) clean -C $(LIBFTDIR)
	rm -rf $(OBJDIR)

fclean: clean
	$(MAKE) fclean -C $(LIBFTDIR)
	rm -f $(NAME)

re: fclean all

# ========= BONUS (可选) =======
debug:
	$(MAKE) CFLAGS="-g3 -fsanitize=address -Wall -Wextra -Werror" re

# ========= PHONY RULES ========
.PHONY: all clean fclean re
