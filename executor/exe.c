#include"exe.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}


/**
 * resolve_path:
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
    return (NULL); // nessun path valido trovato
}


char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
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


// NOTE - Continua da qui (resolve_path)
/**
 * resolve_cmd:
 * Cerca il path assoluto di un comando se non contiene '/'.
 * Alternativa a resolve_path(), usa ft_strjoin_three per evitare leak.
 * Itera sulle directory in $PATH finché trova un file eseguibile.
 * Restituisce stringa heap (da freeare) o NULL se fallisce.
 */
char *resolve_cmd(char *cmd, t_env *env)
{
    char    *path_var = get_env_value("PATH", env);
	char	**paths;
	char	*full_path;
	int		i;

	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd); // custom: evita leak
		if (!full_path)
			break;
		if (access(full_path, X_OK) == 0)
		{
			free_split_all(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_split_all(paths);
	return (NULL);
}



//---------------------------------------------------------------------------------------------------------------------------------


size_t	ft_strlen(const char *s)
{
	size_t i = 0;
	while (s && s[i])
		i++;
	return (i);
}


char	*ft_strdup(const char *s)
{
	size_t	len = ft_strlen(s);
	char	*dup = malloc(len + 1);
	size_t	i = 0;

	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}


static size_t	count_words(const char *s, char c)
{
	size_t i = 0, words = 0;

	while (s && s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			words++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (words);
}


static char	*dup_word(const char *s, char c)
{
	size_t len = 0;
	while (s[len] && s[len] != c)
		len++;
	char *word = malloc(len + 1);
	size_t i = 0;
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}


char	**ft_split(char const *s, char c)
{
	size_t	i = 0, j = 0;
	char	**res;

	if (!s)
		return (NULL);
	res = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!res)
		return (NULL);
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			res[j] = dup_word(&s[i], c);
			if (!res[j++])
			{
				free_split_all(res);
				return (NULL);
			}
			while (s[i] && s[i] != c)
				i++;
		}
	}
	res[j] = NULL;
	return (res);
}


char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*joined;
	size_t	i;

	if (!s1 || !s2)
		return (NULL);
	len1 = strlen(s1);
	len2 = strlen(s2);
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (NULL);
	i = 0;
	while (*s1)
		joined[i++] = *s1++;
	while (*s2)
		joined[i++] = *s2++;
	joined[i] = '\0';
	return (joined);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------


/**
 * is_builtin (da estendere):
 * Ritorna 1 se il comando è uno dei built-in riconosciuti.
 * Serve per distinguere built-in da binari prima del fork.
 * Va tenuto in sync con exec_builtin().
 */
int is_builtin(char *cmd) {
    if (!cmd) return 0;
    return (!strcmp(cmd, "cd") || !strcmp(cmd, "echo") || !strcmp(cmd, "pwd"));
}


/**
 * exec_builtin (anchessa da estendere):
 * Esegue un built-in. `is_child` determina se uscire o no dal processo.
 * Deve ricevere l’array degli argomenti e un doppio puntatore a env.
 * Solo i built-in implementati vengono eseguiti, altrimenti ritorna 1.
 */
int exec_builtin(char **args, t_env **env, int is_child) {
    (void)env; (void)is_child;
    if (!args || !args[0])
        return 1;
    if (!strcmp(args[0], "pwd"))
        return exe_pwd(args, env, is_child);
    return 1;
}


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


char *get_env_value(char *key, t_env *env) {
    if (!key)
        return NULL;
    while (env) {
        if (env->exported && strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}


char *join_key_value(char *key, char *value) {
    size_t key_len, val_len;
    char *str;

    if (!key || !value)
        return NULL;

    key_len = strlen(key);
    val_len = strlen(value);

    str = malloc(key_len + val_len + 2); // '=' + '\0'
    if (!str)
        return NULL;

    memcpy(str, key, key_len);
    str[key_len] = '=';
    memcpy(str + key_len + 1, value, val_len);
    str[key_len + 1 + val_len] = '\0';

    return str;
}


void free_array(char **arr) {
    if (!arr) return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}


int env_size(t_env *env) {
    int count = 0;
    while (env) {
        if (env->exported)
            count++;
        env = env->next;
    }
    return count;
}


/**
 * apply_redirects:
 * Applica in ordine la lista di redirect del nodo (>, >>, <, <<).
 * Usa dup2() per rimpiazzare STDIN/STDOUT con il file aperto.
 * Il file di heredoc viene già preconfigurato dal parser e usato direttamente.
 * Se un redirect fallisce, stampa errore e ritorna 1.
 */
int apply_redirects(t_redir *red) {
    int fd;
    

    while (red) {
        if (red->type == R_IN)
            fd = open(red->file, O_RDONLY);
        else if (red->type == R_OUT)
            fd = open(red->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (red->type == R_APPEND)
            fd = open(red->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (red->type == R_HEREDOC)
            fd = red->fd; // Assume already prepared
        else
            return (1);

        if (fd < 0) {
            perror("redirect open error");
            return (1);
        }
		
		int target; // STDIN_FILENO;=0, STDOUT_FILENO;=1,STDERR_FILENO;=2
		if (red->type == R_IN || red->type == R_HEREDOC)
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
        red = red->next;
    }
    return (0);
}


/**
 * exec_child:
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

	if (node->red && apply_redirects(node->red))
		exit(1);

	if (is_builtin(node->cmd_argv[0]))
		exit(exec_builtin(node->cmd_argv, &env, 1));

	char *bin;
	if (ft_strchr(node->cmd_argv[0], '/')) // già path valido
		bin = node->cmd_argv[0];
	else
		bin = resolve_path(node->cmd_argv[0], env); // NOTE - Continua da qui

	if (!bin)
		exit(127);


	char **env_arr = env_to_array(env);
	execve(bin, node->cmd_argv, env_arr);

	perror("execve");
	free_array(env_arr);
	exit(126);
}


/**
 * create_pipe_if_needed:
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


/**
 * handle_builtin_if_applicable:
 * Se il nodo è un built-in **senza next**, lo esegue nel processo principale.
 * Salva temporaneamente STDIN/STDOUT se ci sono redirect.
 * Serve per comandi che devono modificare lo stato della shell (cd, export, ecc.).
 * Restituisce 1 se il comando è stato gestito (e quindi non va forkato).
 */
int handle_builtin_if_applicable(t_node *node, t_env **env, int *last_status)
{
	if (is_builtin(node->cmd_argv[0]) && !node->next) // built-in solo se ultimo
	{
		int save_out = save_fd(STDOUT_FILENO);
		int save_in = save_fd(STDIN_FILENO);

		if (node->red && apply_redirects(node->red))
			*last_status = 1;
		else
			*last_status = exec_builtin(node->cmd_argv, env, 0);

		switch_fd(save_out, STDOUT_FILENO);
		switch_fd(save_in, STDIN_FILENO);
		close(save_out);
		close(save_in);
		return 1;
	}
	return 0;
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


// NOTE - Continua da qui
/**
 * env_to_array:
 * Converte la lista linkata di variabili d’ambiente in un array "KEY=VALUE",
 * da passare come terzo argomento a execve().
 */
char **env_to_array(t_env *env)
{
	int count = env_size(env); // funzione tua che conta nodi
	char **arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return NULL;

	int i = 0;
	while (env)
	{
		if (env->exported)
		{
			arr[i] = join_key_value(env->key, env->value); // "VAR=val"
			if (!arr[i++])
			{
				free_array(arr);
				return NULL;
			}
		}
		env = env->next;
	}
	arr[i] = NULL;
	return arr;
}


//-------------------------------------------------------------------------------------------------------------------------------------------


int switch_fd(int from, int to)
{
	if (from != to)
	{
		if (dup2(from, to) == -1)
		{
			perror("dup2");
			return 1;
		}
	}
	return 0;
}


int save_fd(int fd)
{
	int clone = dup(fd);
	if (clone == -1)
	{
		perror("dup");
		exit(EXIT_FAILURE); // conviene exit qua? non credo
	}
	return clone;
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


int	open_outfile(const char *filename)                      	// O_CREAT	Se il file non esiste, lo crea
{                                                           		// O_WRONLY	Scrivere solo, non legge
	int	fd;                                                 	// O_TRUNC	Azzera il file se già esiste (come >)
                                                            		// 0644 Permessi rw-r--r--
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("error opening outfile");
		return -1;                                 	// TOLTA uscita qui
	}
	return fd;
}


int save_stdout(void)
{
	int clone = dup(1);
	if (clone == -1)
	{
		perror("dup stdout");
		exit(EXIT_FAILURE);                                	// imposta return
	}
	return clone;
}


int exe_btin(int n, char **args, t_env **env, int prev_exit)
{
	(void)env;
	(void)prev_exit;
	// if (n == CMD_ECHO)
	// 	return exe_echo(args);
	// else if (n == CMD_CD)
	// 	return exe_cd(args, env);
	//else 
	if (n == CMD_PWD)
		return exe_pwd(args, env, prev_exit);
	// else if (n == CMD_EXPORT)
	// 	return exe_export(args, env);
	// else if (n == CMD_UNSET)
	// 	return exe_unset(args, env);
	// else if (n == CMD_ENV)
	// 	return exe_env(*env);
	// else if (n == CMD_EXIT)
	// 	return exe_exit(args, prev_exit, env); // può fare free e exit() direttamente
	return 0;
}


int executor_loop(t_node *node, t_env *env, int *last_status)
{
	int pipe_fd[2];
	int prev_fd = -1;
	pid_t last_pid = -1;

	while (node)
	{
		if (create_pipe_if_needed(node, pipe_fd))
			return 1;

		if (handle_builtin_if_applicable(node, &env, last_status))
			return 0;
		else {
			pid_t pid = fork();
			if (pid == -1)
			{
				perror("fork");
				return 1;
			}
			if (pid == 0)
				exec_child(node, pipe_fd, prev_fd, env);
			else
				last_pid = pid;

			if (prev_fd != -1)
				close(prev_fd);
			if (node->next)
			{
				prev_fd = pipe_fd[0];
				close(pipe_fd[1]);
			}
			node = node->next;
		}
	}

	int status;
	pid_t pid;
	while ((pid = wait(&status)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				*last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*last_status = 128 + WTERMSIG(status);
		}
	}
	return *last_status;
}

int main(void)
{
	t_node *head = create_test_nodes();     // env inizialmente vuoto
	t_env *env = NULL;                      // scegli se array o lista
	int status = 0;

	executor_loop(head, env, &status);
	debug_print_env(env);
	debug_status(status);

	free_test_nodes(head);
	return 0;
}