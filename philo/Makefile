NAME = philo
SRCS = philo.c
OBJS = ${SRCS:.c=.o}

all: $(NAME)

$(OBJS): %.o : %.c
	gcc -c -Wall -Wextra -Werror $< -o $@

$(NAME): $(OBJS)
	gcc -Wall -Wextra -Werror $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

test:
	echo "Write some tests"
	norminette $(SRCS)

.PHONY: all clean fclean re test
