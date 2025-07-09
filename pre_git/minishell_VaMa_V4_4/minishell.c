/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/05/28 12:50:15 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// bool	exit_status = false; //Lo aggiorni dopo l'esecuzione di un comando.
// per exit questo gia lo faccio con la struct. la globale serve per i segnali.
// ovvero questa:
volatile sig_atomic_t g_sigint = 0; 


// funzione per testare che controlla i token (DA RIMUOVERE)
void	ft_print_token(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		printf("Token[%d]: Type=%d, Value='%s'\n", i++, token->type, token->value);
		token = token->next;
	}
}

void	ft_print_nodes(t_node *node)
{
	int i;
	int index = 0;
	t_redir *redir;

	while (node)
	{
		printf("Comando %d:\n", ++index);
		if (node->argv)
		{
			for (i = 0; node->argv[i]; i++)
				printf("  argv[%d]: %s\n", i, node->argv[i]);
		}
		else
		{
			printf("  (nessun argomento)\n");
		}
		redir = node->redirs;
		while (redir)
		{
			 char *symbol;
			switch (redir->type)
			{
				case TK_REDIR_IN_2:
					symbol = "<";
					break;
				case TK_REDIR_OUT_3:
					symbol = ">";
					break;
				case TK_REDIR_APPEND_4:
					symbol = ">>";
					break;
				case TK_HEREDOC_5:
					symbol = "<<";
					break;
				default:
					symbol = "(tipo sconosciuto)";
					break;
			}
			// printf("  Redirect: %s %s (fd: %d)\n", symbol, redir->filename, redir->fd);
			printf("  Redirect: %s %s \n", symbol, redir->filename);
			redir = redir->next;
		}
		node = node->next;
		if (node)
			printf("  [Pipe ->]\n");
	}
}


int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    char *input;
    t_token *token;
    t_node *node;
    t_shell_state state;


    init_shell_state(&state, envp);
	setup_signals();
    token = NULL;
    node = NULL;
    while (!state.should_exit)
    {
        g_sigint = 0;
        input = readline("minishell$ "); // prima controllo sull input poi controllo su readline
		if (!input) // ctrl + d 
        {
            printf("exit\n");
            break;
        }
		if (input[0] == '\0')  // linea vuota: solo invio NUOVO
		{
			free(input);
			//input = NULL;
			continue;
		}
        if (input && *input)
            add_history(input);
        if (g_sigint) // Gestione SIGINT dopo readline
		{
			g_sigint = 0;
			if (input && *input == '\0') {
				free(input);
				input = NULL;
			}
			continue;
		}
        token = ft_tokenize(&state, token, input);
        if (!token)
        {
            // ft_free_token(token);
            // rl_clear_history();
			free(input); // AGGIUNTO DA VALE
            //exit(1); // AGGIUNTO DA VALE
			continue;
        }
        //ft_print_token(token);
        if(ft_check_syntax(token)) // AGGIUNTO DA VALE
		{
			ft_free_token(token); // free nel caso di sintassi errata
			free(input);         // // token ed history è già stato liberato dentro ft_error libero input
			continue;
		}
        node = ft_node(token);
        //ft_print_nodes(node);
        disable_signals();
        executor_loop(node, &state);  // cambiato &env e &status in &state
        setup_signals();
		// debug_print_env(state.env);
        // debug_status(state.last_status);
        free(input);
		//ft_print_token(token);
        ft_free_token(token);
        ft_free_nodes(node);
    }
    cleanup_shell_state(&state); // controlla double free
    rl_clear_history();
    
    return state.exit_code;
}
