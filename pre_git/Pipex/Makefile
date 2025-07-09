# Nome della libreria statica
NAME = pipex

# Compilatore e flag, -c compila i file .c e crea i file oggetto singolarmente
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Regola per compilare tutto
all: $(NAME)

# Lista dei file sorgenti e oggetti
SRCS = pipex.c pipex_utils.c pipex_error_free.c ft_split.c

OBJS = $(SRCS:.c=.o)

# Regola per creare l'eseguibile
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)   
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
