NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS =	pipex.c \
		ft_split.c \
		utils1.c \
		utils2.c \
		utils3.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
