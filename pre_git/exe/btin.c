#include"exe.h" //!SECTION                                                            t_env {
// Questa funzione cerca "PWD" o "OLDPWD" nella lista env e aggiornare il valore. 	        key   = "PATH";
// Se la variabile non esiste, la aggiunge.	                                                value = "/usr/bin:/bin";
void set_env_var(t_env **env, const char *key, const char *value) // 	                    next  = ...; // prossimo env
{                                                                //                         }
	t_env *cur = *env; // t_env (a value serve mtxtolist / listtomtx?)

	while (cur)
	{
		if (strcmp(cur->key, key) == 0) //ft_
		{
			free(cur->value);
			cur->value = ft_strdup(value);
			return;
		}
		cur = cur->next;
	}

	// non trovato, aggiungo
	t_env *new = malloc(sizeof(t_env));
	if (!new)
		return ; // gestione out-of-mem
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = *env;
	*env = new;
}


int exe_cd(char **args, t_env **env) //accorcia
{
	char	oldpwd[PATH_MAX];
	char	newpwd[PATH_MAX];
	char	*target;

	if (!getcwd(oldpwd, sizeof(oldpwd)))
	{
		perror("cd getcwd (OLDPWD)");
		return (1);
	}
	target = args[1];
	if (!target)
	{
		target = get_env_value("HOME", *env);
		if (!target)
		{
			write(2, "cd: HOME not set\n", 18);
			return (1);
		}
	}
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	if (!getcwd(newpwd, sizeof(newpwd)))
	{
		perror("cd getcwd (PWD)");
		return (1);
	}
	set_env_var(env, "OLDPWD", oldpwd);
	set_env_var(env, "PWD", newpwd);

	return (0);
}



int exe_pwd(char **args, t_env **env)
{
	(void)args;
	(void)env;

	char cwd[PATH_MAX + 1];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}	

/**
 * Ritorna 1 se il comando deve essere eseguito nel processo padre.
 * Questi comandi modificano lo stato della shell e devono essere
 * eseguiti solo se sono l'ultimo della pipe (senza next).
 */
int is_parent_builtin(char *cmd)
{
    if (!cmd) return 0;
    return (!strcmp(cmd, "cd") || 
            !strcmp(cmd, "export") || 
            !strcmp(cmd, "unset") || 
            !strcmp(cmd, "exit"));
}

/**
 * Ritorna 1 se il comando deve essere sempre eseguito nel figlio.
 * Questi comandi non modificano lo stato della shell.
 */
int is_child_builtin(char *cmd)
{
    if (!cmd) return 0;
    return (!strcmp(cmd, "echo") || 
            !strcmp(cmd, "pwd") || 
            !strcmp(cmd, "env"));
}

/**
 * Determina se il comando deve essere eseguito nel processo padre.
 * Un builtin viene eseguito nel padre solo SE:
 * 1. È un parent_builtin (cd, export, unset, exit)
 * 2. È l'ultimo comando della pipe (node->next == NULL)
 */
int should_execute_in_parent(t_node *node)
{
    return (is_parent_builtin(node->cmd_argv[0]) && !node->next);
}

/**
 * Aggiorna is_builtin per includere tutti i builtin
 */
int is_builtin(char *cmd)
{
    if (!cmd) return 0;
    return (is_parent_builtin(cmd) || is_child_builtin(cmd));
}


///--unset/export--///


/**
 * Trova un nodo nell'environment per chiave.
 * Ritorna il nodo se trovato, NULL altrimenti.
 */
t_env *find_env_node(t_env *env, const char *key)
{
    while (env)
    {
        if (strcmp(env->key, key) == 0)
            return env;
        env = env->next;
    }
    return NULL;
}

/**
 * Crea un nuovo nodo environment con key e value.
 * exported indica se la variabile è visibile ai processi figli.
 */
t_env *create_env_node(const char *key, const char *value, int exported)
{
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return NULL;
    
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    if (!new_node->key || !new_node->value)
    {
        free(new_node->key);
        free(new_node->value);
        free(new_node);
        return NULL;
    }
    
    new_node->exported = exported;
    new_node->next = NULL;
    return new_node;
}

/**
 * Rimuove un nodo dall'environment per chiave.
 * Ritorna 0 se rimosso, 1 se non trovato.
 */
int remove_env_node(t_env **env, const char *key)
{
    t_env *current = *env;
    t_env *prev = NULL;
    
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env = current->next;
            
            free(current->key);
            free(current->value);
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return 1; // Non trovato
}

/**
 * exe_export:
 * Implementa il comando export.
 * Sintassi: export [VAR=value] [VAR2=value2] ...
 * Senza argomenti: mostra tutte le variabili esportate
 * Con argomenti: esporta le variabili specificate
 */
int exe_export(char **args, t_env **env)
{
    int i = 1;
    
    // Se nessun argomento, mostra tutte le variabili esportate
    if (!args[1])
    {
        t_env *current = *env;
        while (current)
        {
            if (current->exported)
                printf("declare -x %s=\"%s\"\n", current->key, current->value);
            current = current->next;
        }
        return 0;
    }
    
    // Processa ogni argomento
    while (args[i])
    {
        char *equals = strchr(args[i], '=');
        
        if (equals)
        {
            // Caso: VAR=value
            *equals = '\0'; // Separa temporaneamente
            char *key = args[i];
            char *value = equals + 1;
            
            t_env *existing = find_env_node(*env, key);
            if (existing)
            {
                // Aggiorna variabile esistente
                free(existing->value);
                existing->value = ft_strdup(value);
                existing->exported = 1;
            }
            else
            {
                // Crea nuova variabile
                t_env *new_node = create_env_node(key, value, 1);
                if (new_node)
                {
                    new_node->next = *env;
                    *env = new_node;
                }
            }
            *equals = '='; // Ripristina la stringa
        }
        else
        {
            // Caso: VAR (solo esporta variabile esistente)
            t_env *existing = find_env_node(*env, args[i]);
            if (existing)
                existing->exported = 1;
            else
            {
                // Crea variabile vuota esportata
                t_env *new_node = create_env_node(args[i], "", 1);
                if (new_node)
                {
                    new_node->next = *env;
                    *env = new_node;
                }
            }
        }
        i++;
    }
    return 0;
}

/**
 * Sintassi: unset VAR1 [VAR2] [VAR3] ...
 * Rimuove le variabili specificate dall'environment.
 */
int exe_unset(char **args, t_env **env)
{
    int i = 1;
    int exit_status = 0;
    
    if (!args[1])
    {
        write(2, "unset: not enough arguments\n", 28);
        return 1;
    }
    
    while (args[i])
    {
        if (remove_env_node(env, args[i]) != 0)
            printf("nodo non rimosso");

        i++;
    }
    return exit_status;
}

/**
 * Mostra tutte le variabili esportate.
 * Questo è un builtin "del figlio" perché non modifica lo stato.
 */
int exe_env(t_env *env)
{
    while (env)
    {
        if (env->exported)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
    return 0;
}