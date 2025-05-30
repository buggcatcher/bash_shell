#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

enum e_btin { CMD_ECHO, CMD_CD, CMD_PWD };


int switch_fd(int from, int to)
{
	if (dup2(from, to) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);                               	// conviene uscire qua?
	}
	return (0);
}


typedef struct s_env {
	char *key;
	char *value;
	int exported;
	struct s_env *next;
} t_env;


int exe_pwd(char **args, t_env **env, int prev)
{
	(void)args;
	(void)env;
	(void)prev;
	char cwd[PATH_MAX + 1];                               	// security +1 per assicurarsi \0

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);

	else
	{
		fprintf(stderr, "pwd: %s\n", strerror(errno));
		return (1);
	}
	return (0);
}


int	open_outfile(const char *filename)                      // O_CREAT	Se il file non esiste, lo crea
{                                                           // O_WRONLY	Scrivere solo, non legge
	int	fd;                                                 // O_TRUNC	Azzera il file se già esiste (come >)
                                                            // 0644 Permessi rw-r--r--
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("error opening outfile");
		exit(EXIT_FAILURE);                                 // forse poi non conviene uscire qui
	}
	return fd;
}


int save_stdout(void)
{
	int clone = dup(1);
	if (clone == -1)
	{
		perror("dup stdout");
		exit(EXIT_FAILURE);                                // già, forse qui forse non conviene uscire
	}
	return clone;
}


int exe_btin(int n, char **args, t_env **env, int prev)	   // senza fork per built-in serve fare manualmente 
{
	if (n == CMD_PWD)
		return exe_pwd(args, env, prev);
	return 0;
}


int main(void)
{
	int fd_out = open_outfile("out.txt");
	int clone_stdout = save_stdout();

	switch_fd(fd_out, 1); 	        // tutto ciò che normalmente verrebbe stampato sul terminale tramite stdout finisce nel file out.txt
	close(fd_out);
                           
	char *argv[] = { "pwd", NULL }; // senza fork per built-in serve impostare manualmente nodi ed env
	t_env *env_list = NULL;
	exe_btin(CMD_PWD, argv + 1, &env_list, 0);

	switch_fd(clone_stdout, 1);     // ripristina stdout alla copia fatta con dup(1)
	close(clone_stdout);

	printf("end transmission\n");
	return 0;
}
