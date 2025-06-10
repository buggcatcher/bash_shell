#include "exe.h"
#include <stdlib.h>
#include <string.h>

// Lista di nodi simulati che mia arrivano dal parser


// t_node *create_test_nodes(void)
// {
//     t_node *n1 = malloc(sizeof(t_node));
//     t_node *n2 = malloc(sizeof(t_node));

//     if (!n1 || !n2)
//         return NULL;

//     n1->cmd_argv = malloc(sizeof(char *) * 2);
//     if (!n1->cmd_argv)
//         return NULL;
//     n1->cmd_argv[0] = strdup("pwd");
//     n1->cmd_argv[1] = NULL;
//     n1->red = NULL;
//     n1->next = n2;

//     n2->cmd_argv = malloc(sizeof(char *) * 3);
//     if (!n2->cmd_argv)
//         return NULL;
//     n2->cmd_argv[0] = strdup("wc");
//     n2->cmd_argv[1] = strdup("-l");
//     n2->cmd_argv[2] = NULL;
//     n2->red = NULL;
//     n2->next = NULL;

//     return n1;
// }

t_node *create_test_nodes(void)
{
    t_node *n1 = malloc(sizeof(t_node));
    t_node *n2 = malloc(sizeof(t_node));

    if (!n1 || !n2)
        return NULL;

    n1->cmd_argv = malloc(sizeof(char *) * 2);
    if (!n1->cmd_argv)
        return NULL;
    n1->cmd_argv[0] = strdup("env");
    n1->cmd_argv[1] = NULL;
    n1->red = NULL;
    n1->next = n2;

    n2->cmd_argv = malloc(sizeof(char *) * 3);
    if (!n2->cmd_argv)
        return NULL;
    n2->cmd_argv[0] = strdup("wc");
    n2->cmd_argv[1] = strdup("-l");
    n2->cmd_argv[2] = NULL;
    n2->red = NULL;
    n2->next = NULL;

    return n1;
}

// t_node *create_test_nodes(void)
// {
//     t_node *n1 = malloc(sizeof(t_node));
//     t_node *n2 = malloc(sizeof(t_node));

//     if (!n1 || !n2)
//         return NULL;

//     n1->cmd_argv = malloc(sizeof(char *) * 3);
//     if (!n1->cmd_argv)
//         return NULL;
//     n1->cmd_argv[0] = strdup("export");
//     n1->cmd_argv[1] = strdup("VARIABILE=funziona");
//     n1->cmd_argv[2] = NULL;
//     n1->red = NULL;
//     n1->next = NULL;

//     return n1;
// }

///--ovviamente da qui in poi non funzionano perche non lo eseguo nel main loop del parser che maniene la memoria.

// t_node *create_test_nodes(void)
// {
//     t_node *n1 = malloc(sizeof(t_node));
//     if (!n1)
//         return NULL;

//     n1->cmd_argv = malloc(sizeof(char *) * 2);
//     if (!n1->cmd_argv)
//         return NULL;
//     n1->cmd_argv[0] = strdup("echo");
//     n1->cmd_argv[1] = NULL;
    
//     // Creazione redirezione
//     t_redir *red = malloc(sizeof(t_redir));
//     if (!red)
//         return NULL;
//     red->file = strdup("file");
//     red->type = R_APPEND;
//     red->next = NULL;
//     n1->red = red;
    
//     n1->next = NULL;

//     return n1;
// }



void free_test_nodes(t_node *head)
{
    t_node *tmp;
    int i;

    while (head)
    {
        i = 0;
        while (head->cmd_argv && head->cmd_argv[i])
        {
            free(head->cmd_argv[i]);
            i++;
        }
        free(head->cmd_argv);
        tmp = head->next;
        free(head);
        head = tmp;
    }
}

