/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/28 20:51:50 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NEW
void	ft_free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->key)
            free(tmp->key);
        if (tmp->value)
            free(tmp->value);
		free(tmp);
	}
}


// NEW new meow new
t_node *ft_free_nodes(t_node *head)
{
    t_node *tmp;
    
    while (head)
    {
        tmp = head;
        head = head->next;
        
        ft_free_argv(tmp->argv);
        ft_free_redirs(tmp->redirs);
        free(tmp);
    }
    return (NULL);
}



void	ft_free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		if (tmp->value)
			free(tmp->value);

		free(tmp);
	}
}

// t_node	*ft_free_nodes(t_node *head)
// {
// 	t_node	*tmp;

// 	while (head)
// 	{
// 		tmp = head;
// 		head = head->next;
// 		ft_free_argv(tmp->argv);
// 		ft_free_redirs(tmp->redirs);
// 		free(tmp);
// 	}
// 	return (NULL);
// }

void	ft_free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

// questa è buona, solo che sotto c'è la versione commentata per debuggare
// void ft_free_redirs(t_redir *redir)
// {
//     t_redir *next_redir;
    
//     while (redir)
//     {
//         next_redir = redir->next;
//         free(redir->filename);

//         if (redir->heredoc_buffer)
//             ft_free_heredoc_buffer(redir->heredoc_buffer);
    
// 		if (redir->type == TK_HEREDOC_5 && redir->fd > 2)
//         {
//             close(redir->fd);
//         }
//         free(redir);
//         redir = next_redir;
//     }
// }

void ft_free_redirs(t_redir *redir)
{
    t_redir *next_redir;
    
    //printf("=== FT_FREE_REDIRS START ===\n");
    while (redir)
    {
        //printf("Freeing redir: type=%d, fd=%d, filename=%s\n", 
        //       redir->type, redir->fd, redir->filename ? redir->filename : "(null)");
        //printf("redir->heredoc_buffer = %p\n", redir->heredoc_buffer);
        
        next_redir = redir->next;
        //printf("About to free filename\n");
        free(redir->filename);
        //printf("Filename freed\n");

        if (redir->heredoc_buffer)
        {
            //printf("About to free heredoc_buffer\n");
            free_heredoc_buffer(redir->heredoc_buffer);
            //printf("Heredoc buffer freed\n");
        }
        
        //printf("About to free redir struct\n");
        free(redir);
        //printf("Redir struct freed\n");
        redir = next_redir;
    }
    //printf("==== FT_FREE_REDIRS END ====\n");
}

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void free_heredoc_buffer(t_heredoc_buffer *buffer)
{
    if (!buffer)
        return;
    if (buffer->content)
        free(buffer->content);
    free(buffer);
}

void clean_exit(t_node *node, t_env *env, int status)
{
    ft_free_nodes(node);
    ft_free_env(env);
    exit(status);
}
