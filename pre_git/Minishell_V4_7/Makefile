# Nome della libreria statica
NAME = minishell

# Compilatore e flag, -c compila i filename .c e crea i filename oggetto singolarmente
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

# Regola per compilare tutto
all: $(NAME)

# Lista dei filename sorgenti e oggetti
SRCS = utils.c tokenize_mini.c token_type_mini.c error_mini.c utils_mini.c \
itoa_mini.c dollar_mini.c syntax_check_mini.c node_mini.c free_mini.c node_argv_mini.c \
heredoc_mini.c env.c exe_btin.c exe_cd.c exe_exit.c exe_export.c memory.c signals.c \
fd.c split.c libfx.c btin_search.c various.c main.c executor_loop.c system_env.c \
redirects.c resolve_path.c child.c btin_apply.c


OBJS = $(SRCS:.c=.o)

# Regola per creare la libreria statica
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline
	@echo "Eseguibile $(NAME) creato con successo!"

# Regola per compilare i filename .o dai filename .c
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

# Regola per pulire i filename oggetto
clean:
	@rm -f $(OBJS)
	@echo "Pulizia filename oggetto completata!"

# Regola per pulire tutto (filename oggetto + libreria)
fclean: clean
	@rm -f $(NAME)
	@echo "Pulizia completa (incluso $(NAME))!"

# Regola per ricompilare tutto
re: fclean all

val: all
	@valgrind --suppressions=readline.supp --show-leak-kinds=all --leak-check=full --quiet ./$(NAME)

# Evita errori nel caso i nomi delle regole coincidano con filename reali
.PHONY: all clean fclean re