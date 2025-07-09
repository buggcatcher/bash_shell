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

t_token *ft_dquote(t_shell_state *state, t_token **new, char **input)  // MODIFICATO
{
	char	*start;
	char	*buffer;
	int		var;

	start = *input;
	buffer = NULL;
	var = 1;
	(*input)++;
	start = *input;
	ft_check_dquote(state, start);  // MODIFICATO
	while (**input && **input != '"')
	{
		if (var != 2)
			var = ft_check_var(input);
		buffer = ft_create_var(buffer, input, state); // MODIFICATO
	}
	if (var == 1)
		*new = ft_create_token(TK_D_QUOTE_7, start, *input - start);
	else if (var == 2)
		*new = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen_v(buffer));
	if (buffer)
		free(buffer);
	(*input)++;
	return (*new);
}

void ft_check_dquote(t_shell_state *state, char *start) // MODIFICATO
{
    int i = 0;
    while (start[i] && start[i] != '"')
        i++;
    
    if (start[i] != '"')
    {
        state->last_status = 2;  
        //state->syntax_error = true; si potrebbe implementare
        ft_putstr_stderr("Error: unclosed double quotes\n");
    }
}

int	ft_check_var(char **input)
{
	char	*tmp;
	int		start_pos;

	if (**input != '$')
		return (1); // Restituisce 1, caso di TK_WORD_0 o TK_D_QUOTE_7
	tmp = *input + 1; // Puntatore al carattere dopo '$'
	start_pos = 0;  // Posizione relativa dopo '$'
	if (tmp[start_pos] == '?')  // Caso speciale $?
		return (2);  // Restituisce 2, caso di TK_DOLLAR_8
	while (tmp[start_pos] && 
		  ((tmp[start_pos] >= '0' && tmp[start_pos] <= '9') || 
		   (tmp[start_pos] >= 'A' && tmp[start_pos] <= 'Z') ||
		   (tmp[start_pos] >= 'a' && tmp[start_pos] <= 'z') || 
		   tmp[start_pos] == '_'))
		start_pos++;
	if (start_pos == 0)  // Se non abbiamo trovato caratteri validi dopo '$'
		return (1);  // Restituisce 1, caso di TK_WORD_0 o TK_D_QUOTE_7
	else
		return (2);  /// Restituisce 2, caso di TK_DOLLAR_8
}

char	*ft_create_var(char *buffer, char **input, t_shell_state *state) // MODIFICATO
{
	char	*var;
	char	*tmp;

	if (**input == '$')
	{							 // QUI
		var = ft_expand_var(input, state);  // Avanza *input in ft_expand_var
		if (!buffer)
			buffer = var;  // Assegna direttamente se buffer è NULL
		else
		{
			tmp = ft_strjoin(buffer, var); // Concatena
			free(buffer);
			free(var);
			buffer = tmp;
		}
	}
	else
	{
		buffer = ft_append_char(buffer, **input);  // Aggiunge un carattere 
		(*input)++;
	}
	return (buffer);  // Ritorna il nuovo buffer
}
	
char	*ft_expand_var(char **input, t_shell_state *state) // MODIFICATO
{
	char	*start;       // Puntatore all'inizio del nome della variabile
	char	*var_name;    // Nome della variabile (es. "USER")
	char	*var_value;   // Valore della variabile (es. "root")

	(*input)++;  // Salta il carattere '$' (es. da "$USER" → "USER")
	start = *input;  // Memorizza l'inizio del nome (per estrarlo dopo)
	if (**input == '?')  // Caso $?
	{
		(*input)++; // Salta '?'
		return (ft_itoa(state->last_status));  // Restituisci lo status (es. 0), ma senza creare token
	}
	while (**input && ((**input >= '0' && **input <= '9') || (**input >= 'A' && **input <= 'Z') ||// Leggi il nome della variabile, numeri, Lettere maiuscole
			(**input >= 'a' && **input <= 'z') || **input == '_')) // Lettere minuscole, underscore
		(*input)++;
	if (*input == start)  // Se non hai avanzato (nessun carattere valido dopo '$')
		return (ft_strdup("$"));  // Restituisci "$" come stringa allocata
	var_name = ft_strndup(start, *input - start); // Copia il nome (es. "USER")
	var_value = getenv(var_name); // Cerca la variabile d'ambiente e restituisce un puntatore alla stringa del suo valore
	free(var_name); // Libera il nome
	if (!var_value) // Se la variabile non esiste (se getenv fallisce O o nome vuoto)
		var_value = ""; // stringa vuota - Le shell standard (Bash, Zsh, ecc.) trattano le variabili inesistenti come stringhe vuote, non come errori.
	return (ft_strdup(var_value));  // Restituisci una copia allocata del valore
}
