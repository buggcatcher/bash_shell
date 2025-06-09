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

bool	exit_status = false; //Lo aggiorni dopo l'esecuzione di un comando, per esempio:

// int status;
// waitpid(pid, &status, 0);
// if (WIFEXITED(status))
// 	exit_status = WEXITSTATUS(status);  // aggiorna il valore globale


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
	int cmd_index = 0;
	t_redir *redir;

	while (node)
	{
		printf("Comando %d:\n", ++cmd_index);
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
			printf("  Redirect: %s %s (fd: %d)\n", symbol, redir->filename, redir->fd);
			redir = redir->next;
		}
		node = node->next;
		if (node)
			printf("  [Pipe ->]\n");
	}
}


int	main(void)
{
	char	*input;
	t_token	*token;
	t_node	*node;

	token = NULL;
	node = NULL;
	while (1)
	{
		input = readline("minishell$ "); // Se l’utente digita qualcosa e preme Invio, readline() ritorna un puntatore a una stringa allocata dinamicamente contenente quel testo (senza il carattere \n).
		if (!input) // Se l’utente preme Ctrl-D a inizio riga, readline() ritorna NULL.
			break; // il programma termina
		if (input && *input) // se input != NULL e non vuota
			add_history(input); // aggiungere una riga di testo alla cronologia dei comandi.
		token = ft_tokenize(token, input); // conta i token 
		ft_print_token(token); // funzione per testare che controlla i token (DA RIMUOVERE)
		ft_check_syntax(token);
		node = ft_node(token);
		ft_print_nodes(node);
		free(input);
		ft_free_token(token);
		ft_free_nodes(node);
	}
	rl_clear_history(); // NON prende parametri.
	return (0);
}