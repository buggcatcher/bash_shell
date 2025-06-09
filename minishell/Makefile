# Nome della libreria statica
NAME = minishell

# Compilatore e flag, -c compila i file .c e crea i file oggetto singolarmente
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Regola per compilare tutto
all: $(NAME)

# Lista dei file sorgenti e oggetti
SRCS = minishell.c tokenize_mini.c token_type_mini.c error_mini.c utils_mini.c itoa_mini.c dollar_mini.c syntax_check_mini.c node_mini.c free_mini.c node_argv_mini.c

OBJS = $(SRCS:.c=.o)

# Regola per creare la libreria statica
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline
	@echo "Eseguibile $(NAME) creato con successo!"

# Regola per compilare i file .o dai file .c
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Regola per pulire i file oggetto
clean:
	@rm -f $(OBJS)
	@echo "Pulizia file oggetto completata!"

# Regola per pulire tutto (file oggetto + libreria)
fclean: clean
	@rm -f $(NAME)
	@echo "Pulizia completa (incluso $(NAME))!"

# Regola per ricompilare tutto
re: fclean all

# Evita errori nel caso i nomi delle regole coincidano con file reali
.PHONY: all clean fclean re