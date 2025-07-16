/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/16 17:11:00 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Alloca e inizializza una struttura t_heredoc_buffer, predisponendo un buffer di partenza 
per contenere il contenuto degli heredoc.
Usa safe_alloc per gestire la memoria in modo sicuro e imposta i campi iniziali del buffer.
*/
static t_heredoc_buffer *create_heredoc_buffer(void)
{
    t_heredoc_buffer *buffer;

    buffer = safe_alloc(1, sizeof(t_heredoc_buffer), "heredoc buffer struct");
    if (!buffer)
        return (NULL);
    buffer->content = safe_alloc(4096, sizeof(char), "heredoc content buffer");
    if (!buffer->content)
    {
        free(buffer);
        return (NULL);
    }
    buffer->size = 0;
    buffer->capacity = 4096;
    buffer->content[0] = '\0';
    return (buffer);
}

/*
Si occupa di espandere dinamicamente il buffer se quello attuale non ha più spazio sufficiente.
Alloca un nuovo blocco di memoria, copia il contenuto esistente usando ft_memcpy, 
aggiorna i riferimenti nella struttura e libera il vecchio buffer.
*/
static int resize_buffer(t_heredoc_buffer *buffer, size_t new_capacity)
{
    char *new_content;

    new_content = safe_alloc(new_capacity, sizeof(char), "heredoc resized buffer");
    if (!new_content)
        return (1);
    ft_memcpy(new_content, buffer->content, buffer->size);
    new_content[buffer->size] = '\0';
    free(buffer->content);
    buffer->content = new_content;
    buffer->capacity = new_capacity;
    return (0);
}

/*
Prende una linea di input e la aggiunge al buffer. 
Se il buffer non è abbastanza capiente, lo ridimensiona prima di procedere. 
Copia il testo nel buffer e aggiunge manualmente il carattere di newline.
*/
int append_to_buffer(t_heredoc_buffer *buffer, const char *line)
{
    size_t line_len;
    size_t needed_size;
    size_t new_capacity ;
    
    line_len = strlen(line);
    needed_size = buffer->size + line_len + 1;
    if (needed_size >= buffer->capacity)
    {
        new_capacity = buffer->capacity * 2;
        while (new_capacity < needed_size)
            new_capacity *= 2;
        
        if (resize_buffer(buffer, new_capacity) != 0)
            return (1);
    }
    ft_strcpy(buffer->content + buffer->size, line);
    buffer->size += line_len;
    buffer->content[buffer->size] = '\n';
    buffer->size++;
    buffer->content[buffer->size] = '\0';
    return (0);
}

/*
Prende il contenuto del buffer e lo scrive in una pipe anonima, restituendo 
il file descriptor della sua estremità in lettura. 
Serve a simulare l’input da file richiesto dalle redirezioni heredoc.
*/
int create_fd_from_buffer(t_heredoc_buffer *buffer)
{
    int pipefd[2];
    ssize_t bytes_written;
    
    if (pipe(pipefd) == -1)
        return (-1);
    bytes_written = write(pipefd[1], buffer->content, buffer->size);
    if (bytes_written == -1 || (size_t)bytes_written != buffer->size)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    close(pipefd[1]); // chiude scrittura
    return (pipefd[0]); // ritorna lettura
}

/*
Coordina il processo per una sequenza di heredoc consecutivi. 
Crea il buffer, lo riempie con fill_heredoc_buffer, 
genera il file descriptor con create_fd_from_buffer, lo assegna al primo heredoc, 
marca gli altri heredoc della sequenza come già gestiti usando mark_as_skipped. 
Libera sempre la memoria del buffer al termine.
*/
int handle_heredoc_sequence(t_redir *start_redir)
{
    t_heredoc_buffer *buffer;
    int fd;

    buffer = create_heredoc_buffer();
    if (!buffer)
        return (1);
    if (fill_heredoc_buffer(start_redir, buffer) != 0)
    {
        free_heredoc_buffer(buffer);
        return (1);
    }
    fd = create_fd_from_buffer(buffer);
    if (fd < 0)
    {
        free_heredoc_buffer(buffer);
        return (1);
    }
    start_redir->fd = fd;
    mark_as_skipped(start_redir->next);
    free_heredoc_buffer(buffer);
    return (0);
}


