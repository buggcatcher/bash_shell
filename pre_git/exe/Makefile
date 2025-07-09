CC = cc
CFLAGS = -Wall -Wextra -Werror -g
SRC = exe.c btin.c nodes.c debug.c nodes.c utils.c
OBJ = $(SRC:.c=.o)
NAME = exe

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
