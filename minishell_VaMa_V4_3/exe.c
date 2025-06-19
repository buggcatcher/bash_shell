#include "minishell.h"

/**
 * Cerca il path assoluto di un binario usando la variabile d’ambiente PATH.
 * Se il comando contiene '/', lo restituisce direttamente (path assoluto o relativo).
 * In caso contrario, prova a risolverlo in ogni directory di PATH.
 * Usato prima di chiamare execve().
 */
char *resolve_path(char *cmd, t_env *env)
{
    char    **paths;
    char    *path_var;
    char    *full_path;
    size_t  i = 0;

	//debug_path_resolution(cmd, env);
	
    // Se contiene '/' è già un path assoluto o relativo
    if (ft_strchr(cmd, '/')) 
        return (ft_strdup(cmd));

    path_var = get_env_value("PATH", env);
    if (!path_var)
        return (NULL); // PATH non trovato
    paths = ft_split(path_var, ':');
    if (!paths)
        return (NULL); // malloc fallita

    while (paths[i])
    {
        full_path = join_path(paths[i], cmd); // es: "/bin" + "/" + "ls"
        if (!full_path)
        {
            free_split_all(paths);
            return (NULL); // malloc fallita
        }
        if (access(full_path, X_OK) == 0) // path valido ed eseguibile
        {
            free_split_all(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_split_all(paths);
    printf("command not found\n");
    return (NULL); // nessun path valido trovato
}


void free_split_all(char **arr)
{
    size_t i = 0;

    if (!arr)
        return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}



//---------------------------------------------------------------------------------------------------------------------------------


char *join_path(char *dir, char *cmd)
{
    char    *tmp;
    char    *full;

    tmp = ft_strjoin(dir, "/");
    if (!tmp)
        return (NULL);
    full = ft_strjoin(tmp, cmd);
    free(tmp);
    return (full);
}


char *get_env_value(char *key, t_env *env)
{
    if (!key)
        return NULL;
    while (env) {
        if (ft_strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}


char *join_key_value(char *key, char *value)
{
    if (!key || !value)
        return NULL;

    size_t key_len = strlen(key);
    size_t val_len = strlen(value);
    char *str = safe_alloc(key_len + val_len + 2, sizeof(char), "join_key_value"); 

    if (!str)
        return NULL;

    memcpy(str, key, key_len);
    str[key_len] = '=';
    memcpy(str + key_len + 1, value, val_len);
    str[key_len + 1 + val_len] = '\0'; 
	// '=' + '\0' → +2
    return str;
}




/**
 * Processo figlio creato da fork().
 * Imposta i pipe in/out se richiesti, applica redirect, poi esegue il comando.
 * Se è un built-in, la esegue e chiude il processo.
 * Altrimenti chiama execve con path risolto e variabili d’ambiente trasformate in array.
 * In caso di errore, esce con codice 126 o 127 (standard bash).
 */
void exec_child(t_node *node, int pipe_out[2], int pipe_in, t_env *env)
{
	// Pipe in → STDIN
	if (pipe_in != -1)
		switch_fd(pipe_in, STDIN_FILENO);
	// Pipe out → STDOUT solo se c'è un next
	if (node->next)
		switch_fd(pipe_out[1], STDOUT_FILENO);

	// Chiudi i pipe inutili
	if (pipe_out[0] != -1)
		close(pipe_out[0]);
	if (pipe_out[1] != -1)
		close(pipe_out[1]);
	if (pipe_in != -1)
		close(pipe_in);

	if (node->redirs && apply_redirects(node->redirs))
		exit(1);

	if (is_builtin(node->argv[0]))
		exit(exec_builtin(node->argv, &env));

	char *bin;
	if (ft_strchr(node->argv[0], '/')) // già path valido
		bin = node->argv[0];
	else
		bin = resolve_path(node->argv[0], env); // NOTE - Continua da qui

	if (!bin)
		exit(127);

	char **env_arr = env_to_array(env);
	execve(bin, node->argv, env_arr);

	perror("execve");
	free_array(env_arr);
	exit(127); // MODIFICATO DA VALE : DA 126 A 127 PER ERRORE SU COMANDO NON ESISTENTE
}





int spawn_forked_command(t_node *node, int pipe_fd[2], int prev_fd, t_env *env)
{
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		exec_child(node, pipe_fd, prev_fd, env);
	return (0);
}


/**
 * Converte la lista linkata di variabili d’ambiente in un array "KEY=VALUE",
 * da passare come terzo argomento a execve().
 */
char **env_to_array(t_env *env)
{
    int count = env_size(env); // conta variabili esportate
    char **arr = safe_alloc(count + 1, sizeof(char *), "env_to_array");
    if (!arr)
        return NULL;

    int i = 0;
    while (env)
    {
        if (env->exported)
        {
            arr[i] = join_key_value(env->key, env->value);
            if (!arr[i])
            {
                free_array(arr);
                return NULL;
            }
            i++;
        }
        env = env->next;
    }
    arr[i] = NULL;
    return arr;
}


/**
 * Esegue un built-in. `is_child` determina se uscire o no dal processo.
 * Deve ricevere l’array degli argomenti e un doppio puntatore a env.
 * Solo i built-in implementati vengono eseguiti, altrimenti ritorna 1.
 * Esegue un built-in in base al contesto (padre o figlio).
 */
int exec_builtin(char **args, t_env **env)
{
    if (!args || !args[0])
        return 1;
    
    // Builtin del padre
    if (!ft_strcmp(args[0], "cd"))
        return exe_cd(args, env);
    else if (!ft_strcmp(args[0], "export"))
        return exe_export(args, env);
    else if (!ft_strcmp(args[0], "unset"))
        return exe_unset(args, env);
    // else if (!ft_strcmp(args[0], "exit"))
    //     return exe_exit(args, env);
    
    // Builtin del figlio
    else if (!ft_strcmp(args[0], "pwd"))
        return exe_pwd(args, env);
    else if (!ft_strcmp(args[0], "env"))
        return exe_env(*env);
    else if (!ft_strcmp(args[0], "echo"))
        return exe_echo(args);   
    return 1;
}


/**
 * Applica in ordine la lista di redirect del nodo (>, >>, <, <<).
 * Usa dup2() per rimpiazzare STDIN/STDOUT con il filename aperto.
 * Il filename di heredoc viene già preconfigurato dal parser e usato direttamente.
 * Se un redirect fallisce, stampa errore e ritorna 1.
 */
// int apply_redirects(t_redir *redirs)
// {
//     int fd;
    

//     while (redirs) {
//         if (redirs->type == TK_REDIR_IN_2)
//             fd = open(redirs->filename, O_RDONLY);
//         else if (redirs->type == TK_REDIR_OUT_3)
//             fd = open(redirs->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         else if (redirs->type == TK_REDIR_APPEND_4)
//             fd = open(redirs->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
//         else if (redirs->type == TK_HEREDOC_5)
//         {
//             fd = -2; // AGGIUNTO DA VALE, iizializzata la variabile per evitare memry leak in caso di heredoc, inizializzata a -2 cosi da poter tenere il controddlo di errore apertura fd (-1) negli altri casi 
//             //fd = redirs->fd; // Assume already prepared
//             ft_handle_heredoc(redirs);
//         }
//         else
//             return (1);

//         if (fd == -1)
//         {
//             perror("redirect open error");
//             return (1);
//         }
		
// 		int target; // STDIN_FILENO;=0, STDOUT_FILENO;=1,STDERR_FILENO;=2
// 		if (redirs->type == TK_REDIR_IN_2 || redirs->type == TK_HEREDOC_5)
// 			target = STDIN_FILENO;
// 		else
// 			target = STDOUT_FILENO;

// 		if (redirs->type == TK_REDIR_IN_2 || redirs->type == TK_REDIR_OUT_3 || redirs->type == TK_REDIR_OUT_3 || redirs->type == TK_REDIR_APPEND_4) // AGGIUNTO DA VALE per evitare memory leak in caso di heredoc
// 		{
//             if (switch_fd(fd, target) != 0)
// 		    {
// 			    close(fd);
// 			    return (1);
// 		    }
// 		// Lasciare questo commento
// 		// chiude solo se fd è diverso da target
// 		// if (fd != target)
// 		// 	close(fd);
// 		close(fd);
//         }
//         redirs = redirs->next;
//     }
//     return (0);
// }

int apply_redirects(t_redir *redirs)
{
    int fd;
    

    while (redirs) {
        if (redirs->type == TK_REDIR_IN_2)
            fd = open(redirs->filename, O_RDONLY);
        else if (redirs->type == TK_REDIR_OUT_3)
            fd = open(redirs->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (redirs->type == TK_REDIR_APPEND_4)
            fd = open(redirs->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (redirs->type == TK_HEREDOC_5)
        {
            //fd = redirs->fd; // Assume already prepared
            if (ft_handle_heredoc(redirs) != 0)   // This function sets redirs->fd (or redirs->fd[0])?
                return (1);   // error
            fd = redirs->fd;  
        }
        else
            return (1);

        if (fd < 0) {
            perror("redirect open error");
            return (1);
        }
		
		int target; // STDIN_FILENO;=0, STDOUT_FILENO;=1,STDERR_FILENO;=2
		if (redirs->type == TK_REDIR_IN_2 || redirs->type == TK_HEREDOC_5)
			target = STDIN_FILENO;
		else
			target = STDOUT_FILENO;

		if (switch_fd(fd, target) != 0)
		{
			close(fd);
			return (1);
		}
		// Lasciare questo commento
		// chiude solo se fd è diverso da target
		// if (fd != target)
		// 	close(fd);
		close(fd);
        redirs = redirs->next;
    }
    return (0);
}


/**
 * handle_builtin_if_applicable: rinomina (next no)
 * Se il nodo è un built-in **senza next**, lo esegue nel processo principale.
 * Salva temporaneamente STDIN/STDOUT se ci sono redirect.
 * Serve per comandi che devono modificare lo stato della shell (cd, export, ecc.).
 * Restituisce 1 se il comando è stato gestito (e quindi non va forkato).
 */
int handle_builtin_if_applicable(t_node *node, t_env **env, int *last_status)
{
	if (is_builtin(node->argv[0]) && !node->next) // built-in solo se ultimo
	{
		int save_out = save_fd(STDOUT_FILENO); // da vedere anche il caso child/parent e s e e' seguito da pipe per l'esecuzione
		int save_in = save_fd(STDIN_FILENO);

		if (node->redirs && apply_redirects(node->redirs))
			*last_status = 1;
		else
			*last_status = exec_builtin(node->argv, env); 

		switch_fd(save_out, STDOUT_FILENO);
		switch_fd(save_in, STDIN_FILENO);
		close(save_out);
		close(save_in);
		return 1;
	}
	return 0;
}


/**
 * create_pipe_if_needed: rinomina (next yes)
 * Se il nodo ha un nodo successivo, crea una pipe.
 * Restituisce pipe_fd[2] con i due estremi, altrimenti imposta -1/-1.
 */
int create_pipe_if_needed(t_node *node, int pipe_fd[2])
{
	if (node->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (0);
}


///--env--///


t_env *init_env_from_system(char **envp)
{
    t_env *env = NULL;
    t_env *new_node;
    char *equals_pos;
    int i = 0;

    while (envp && envp[i])
    {
        equals_pos = strchr(envp[i], '=');
        if (equals_pos)
        {
            new_node = malloc(sizeof(t_env));
            if (!new_node)
                return NULL; // TODO: free existing nodes
            
            *equals_pos = '\0'; // Temporarily split the string
            new_node->key = ft_strdup_m(envp[i]);
            new_node->value = ft_strdup_m(equals_pos + 1);
            new_node->exported = 1; // Le variabili di sistema sono esportate
            *equals_pos = '='; // Restore the string
            
            new_node->next = env;
            env = new_node;
        }
        i++;
    }
    return env;
}

void init_shell_state(t_shell_state *state, char **envp)
{
    state->env = init_env_from_system(envp);
    state->last_status = 0;
    state->should_exit = 0;
    state->exit_code = 0;
}

void cleanup_shell_state(t_shell_state *state)
{
    // Free environment
    t_env *current = state->env;
    t_env *next;
    
    while (current) {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    state->env = NULL;
}

int executor_loop(t_node *node, t_shell_state *state)
{
    int pipe_fd[2];
    int prev_fd = -1;
    pid_t last_pid = -1;

    while (node)
    {
        if (create_pipe_if_needed(node, pipe_fd))
            return 1;

        if (should_execute_in_parent(node))
        {
            // exit
            if (!strcmp(node->argv[0], "exit")) {
                exe_exit(node->argv, state);
                return state->last_status;
            }
            // parent builtin (cd, export, unset)
            int save_out = save_fd(STDOUT_FILENO);
            int save_in = save_fd(STDIN_FILENO);

            if (node->redirs && apply_redirects(node->redirs))
                state->last_status = 1;
            else
                state->last_status = exec_builtin(node->argv, &state->env);

            switch_fd(save_out, STDOUT_FILENO);
            switch_fd(save_in, STDIN_FILENO);
            close(save_out);
            close(save_in);
            
            return state->last_status;
        }
        else
        {   // fork per comandi esterni e child builtin
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                return 1;
            }
            if (pid == 0)
                exec_child(node, pipe_fd, prev_fd, state->env);
            else
                last_pid = pid;

            if (prev_fd != -1)
                close(prev_fd);
            if (node->next) {
                prev_fd = pipe_fd[0];
                close(pipe_fd[1]);
            }
        }
        node = node->next;
    }

    // Wait per tutti i figli
    int status;
    pid_t pid;
    while ((pid = wait(&status)) > 0) {
        if (pid == last_pid) {
            if (WIFEXITED(status))
                state->last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                state->last_status = 128 + WTERMSIG(status);
        }
    }
    return state->last_status;
}