UNAME_S := ${shell uname}

FLAGS := -Wall -Wextra -Werror

NAME = philo
HEADERS = srcs/philo.h
SRCS = srcs/philo.c srcs/arguments.c srcs/main.c srcs/data.c \
	   srcs/utils.c srcs/forks.c srcs/manager.c
OBJS = ${SRCS:.c=.o}

all: $(NAME)

$(OBJS): %.o : %.c $(HEADERS)
ifeq ($(UNAME_S),Linux)
	gcc -c $(FLAGS) $< -o $@ -lpthread
endif
ifeq ($(UNAME_S),Darwin)
	gcc -c $(FLAGS) $< -o $@
endif

$(NAME): $(OBJS)
ifeq ($(UNAME_S),Linux)
	gcc $(FLAGS) $(OBJS) -o $(NAME) -lpthread
endif
ifeq ($(UNAME_S),Darwin)
	gcc $(FLAGS) $(OBJS) -o $(NAME)
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
