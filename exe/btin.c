#include"exe.h" //!SECTION                                                            t_env {
// Questa funzione cerca "PWD" o "OLDPWD" nella lista env e aggiornare il valore. 	        key   = "PATH";
// Se la variabile non esiste, la aggiunge.	                                                value = "/usr/bin:/bin";
void set_env_var(t_env **env, const char *key, const char *value) // 	                    next  = ...; // prossimo env
{                                                                //                         }
	t_env *cur = *env; // t_env (a value serve mtxtolist / listtomtx?)
	char *new_value;

	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
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


int exe_cd(char **args, t_env **env)
{
	char	oldpwd[PATH_MAX];
	char	newpwd[PATH_MAX];

	if (!args[1] || args[2])
    {
        write(2, "cd: usage: cd [dir]\n", 21);
        return (1);
    }
	if (!getcwd(oldpwd, sizeof(oldpwd)))
    {                                               // salva la directory corrente in oldpwd prima del cambio
		perror("cd getcwd (OLDPWD)");
		return (1);
	}
	if (chdir(args[1]) != 0)
    {                                               // cambia directory alla path specificata (relativa o assoluta)
		perror("cd");
		return (1);
	}
	if (!getcwd(newpwd, sizeof(newpwd)))
    {                                               // salva la nuova directory corrente in newpwd dopo il cambio
		perror("cd getcwd (PWD)");
		return (1);
	}
	set_env_var(env, "OLDPWD", oldpwd);             // aggiorna OLDPWD nell'env
	set_env_var(env, "PWD", newpwd);                // aggiorna PWD nell'env

	return (0);
}


int exe_pwd(char **args, t_env **env, int prev)
{
	(void)args;
	(void)env;
	(void)prev;

	char cwd[PATH_MAX + 1];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
	return (1);
}	
