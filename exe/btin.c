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
 * is_parent_builtin:
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
 * is_child_builtin:
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
 * should_execute_in_parent:
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