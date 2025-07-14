/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_preprocessor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/14 17:29:23 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Legge riga per riga dall’input standard finché non incontra il delimitatore specificato. 
Per ogni riga letta, la passa a append_to_buffer. 
Se l’utente inserisce il delimitatore, la lettura si interrompe.
*/
int read_heredoc_input(char *delimiter, t_heredoc_buffer *buffer)
{
    char *line;
    size_t delimiter_len;
    
    delimiter_len = ft_strlen(delimiter);
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strlen(line) == delimiter_len && 
            ft_strncmp(line, delimiter, delimiter_len) == 0)
        {
            free(line);
            break;
        }
        if (append_to_buffer(buffer, line) != 0)
        {
            free(line);
            return (1);
        }
        free(line);
    }
    return (0);
}

/*
Si occupa di scorrere tutte le redirezioni di un singolo nodo, e per ogni 
redirezione di tipo heredoc che non è ancora stata processata (fd == -1), 
chiama handle_heredoc_sequence per gestirla. 
Se una qualsiasi di queste chiamate fallisce, interrompe l’esecuzione e segnala l’errore.
*/
static int process_node_heredocs(t_node *node)
{
    t_redir *redir = node->redirs;
    
    while (redir)
    {
        if (redir->type == TK_HEREDOC_5 && redir->fd == -1)
        {
            if (handle_heredoc_sequence(redir) != 0)
                return (1);
        }
        redir = redir->next;
    }
    return (0);
}

/*
Cicla su una lista di nodi, e per ciascuno di essi chiama process_node_heredocs, 
quindi gestisce gli heredoc per tutta la catena di nodi. 
Se il processo fallisce per un nodo, ritorna l’errore immediatamente.
*/
// int preprocess_heredocs(t_node *nodes)
// {
//     t_node *current = nodes;
    
//     current = nodes;
//     while (current)
//     {
//         if (process_node_heredocs(current) != 0)
//             return (1);
//         current = current->next;
//     }
//     return (0);
// }

int preprocess_heredocs(t_node *nodes)
{
    t_node *current = nodes;
    
    current = nodes;
    while (current)
    {
        if (process_node_heredocs(current) != 0)
            return (1);
        current = current->next;
    }
    return (0);
}

/*
La funzione fill_heredoc_buffer legge sequenzialmente tutti gli heredoc consecutivi 
a partire da una redirezione, leggendo l’input fino al delimitatore e accumulandolo nel buffer.
Se la lettura fallisce in qualsiasi punto, ritorna un errore.
*/
// int fill_heredoc_buffer(t_redir *start, t_heredoc_buffer *buffer)
// {
//     t_redir *current = start;

//     while (current && current->type == TK_HEREDOC_5)
//     {
//         if (read_heredoc_input(current->filename, buffer) != 0)
//             return (1);
//         current = current->next;
//     }
//     return (0);
// }
//NEW
int fill_heredoc_buffer(t_redir *start, t_heredoc_buffer *buffer)
{
    t_redir *current = start;

    while (current && current->type == TK_HEREDOC_5)
    {
        buffer->size = 0; // RESET del buffer prima di ogni nuovo heredoc
        if (buffer->capacity > 0 && buffer->content)
        {
            buffer->content[0] = '\0';
        }
        if (read_heredoc_input(current->filename, buffer) != 0)
            return (1);
        current = current->next;
    }
    return (0);
}

/*
Scorre tutte le redirezioni heredoc successive e le marca con fd = -2 per indicare 
che sono state già gestite e devono essere saltate nel processamento successivo.
*/
void mark_as_skipped(t_redir *start)
{
    t_redir *current = start;

    while (current && current->type == TK_HEREDOC_5)
    {
        current->fd = -2;
        current = current->next;
    }
}







