UNAME_S := ${shell uname}

NAME = philo
SRCS = philo.c
OBJS = ${SRCS:.c=.o}

all: $(NAME)

$(OBJS): %.o : %.c
ifeq ($(UNAME_S),Linux)
	gcc -c -Wall -Wextra -Werror $< -o $@ -lpthread
endif
ifeq ($(UNAME_S),Darwin)
	gcc -c -Wall -Wextra -Werror $< -o $@
endif

$(NAME): $(OBJS)
ifeq ($(UNAME_S),Linux)
	gcc -Wall -Wextra -Werror $(OBJS) -o $(NAME) -lpthread
endif
ifeq ($(UNAME_S),Darwin)
	gcc -Wall -Wextra -Werror $(OBJS) -o $(NAME)
endif

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

norm:
	norminette $(SRCS)

test: $(NAME)
	@python3.7 tests/test.py

.PHONY: all clean fclean re test norm
