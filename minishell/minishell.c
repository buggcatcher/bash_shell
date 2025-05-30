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

int	main(void)
{
	char	*input;
	t_token	*token;
	
	token = NULL;
	while (1)
	{
		input = readline("minishell$ "); // Se l’utente digita qualcosa e preme Invio, readline() ritorna un puntatore a una stringa allocata dinamicamente contenente quel testo (senza il carattere \n).
		if (!input) // Se l’utente preme Ctrl-D a inizio riga, readline() ritorna NULL.
			break; // il programma termina
		if (input && *input) // se input != NULL e non vuota
			add_history(input); // aggiungere una riga di testo alla cronologia dei comandi.
		token = ft_tokenize(token, input); // conta i token 
		ft_print_token(token); // funzione per testare che controlla i token (DA RIMUOVERE)
		ft_free_token(token);
		free(input);
	}
	rl_clear_history(); // NON prende parametri.
	return (0);
}