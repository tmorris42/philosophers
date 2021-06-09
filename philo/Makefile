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

norm:
	norminette $(SRCS)

test: $(NAME)
	python3 tests/test.py

.PHONY: all clean fclean re test norm
