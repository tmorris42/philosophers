UNAME_S := ${shell uname}

FLAGS := -Wall -Wextra -Werror
DEBUG_FLAGS := $(FLAGS) -g3 -fsanitize=thread
FLAGS = $(DEBUG_FLAGS)

OBJ_DIR := objs
SRC_DIR	:= srcs

NAME = philo
MAKEFILE = Makefile

INCLUDES_DIR = includes
HEADERS = $(INCLUDES_DIR)/philo.h
SRCS = philo.c \
	   arguments.c main.c data.c \
	   utils.c forks.c manager.c \
	   accessors/playing.c \
	   accessors/philo_alive.c \
	   accessors/philo_time_of_last_meal.c \
	   accessors/philo_times_eaten.c
OBJS := $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

all: $(NAME)
ifeq ($(FLAGS), $(DEBUG_FLAGS))
	@echo " "
	@echo "===================================="
	@echo "| WARNING! Debug mode is activated |"
	@echo "===================================="
	@echo " "
endif

$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(HEADERS) $(MAKEFILE)
	@mkdir -p $(dir $@)
ifeq ($(UNAME_S),Linux)
	gcc -c $(FLAGS) -I$(INCLUDES_DIR) $< -o $@ -lpthread
endif
ifeq ($(UNAME_S),Darwin)
	gcc -c $(FLAGS) -I$(INCLUDES_DIR)  $< -o $@
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
