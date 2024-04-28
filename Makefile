NAME = minishell

COMPILER = cc

# FLAGS = -Wall -Wextra -Werror
FLAGS = -g

SRCS = src/minishell.c src/parser.c src/input.c

LIBFT = libft/libft.a

OBJS = $(SRCS:.c=.o)

OBJS_BNS = $(SRCS_BNS:.c=.o)

all: $(NAME)

bonus: all

$(NAME): $(OBJS) $(LIBFT)
		$(COMPILER) $(FLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
		$(MAKE) -C libft

.c.o:
	$(COMPILER) -c $(FLAGS) $< -o $@

clean:
		$(MAKE) -C libft clean
		rm -f $(OBJS)

fclean: clean
		rm -f libft/libft.a
		rm -f $(NAME)

re: fclean all