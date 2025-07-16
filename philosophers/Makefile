NAME = philo

CC = cc 

FLAGS = -Wall -Wextra -Werror

RM = rm -f

SRCS = main.c ft_utils.c parsing.c creat_thread_helper.c creat_thread.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $^ -o $(NAME)

%.o : %.c philo.h
	$(CC) $(FLAGS) -c $< -o $@
clean :
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all